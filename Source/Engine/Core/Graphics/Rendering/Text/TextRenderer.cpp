//////////////////////////////////////////////////////////////////////
#include "TextRenderer.hpp"
#include "Engine/Assets/ImageBank.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/RendererBillboardImages.hpp"
#include "Font.hpp"
#include "MultiLineText.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class TextRenderer::Pimpl
    /*/////////////////////*/
    {
      public:
        void RenderText(
            RenderId, std::string_view, Color, bool, FontSizes, std::string &, SizeF &) const;
        const std::string CreateBlankTexGetName();

        const std::string relFontsPath = "Resources/Fonts/";
        const Color outlineColor = Colors::black;
        std::map<FontSizes, std::shared_ptr<const Font>> fonts;
        std::map<RenderId, std::string> uniqueNameIds;
        int idCounter = 0;
        std::map<RenderId, MultiLineText> multiLines;
    };

    TextRenderer::TextRenderer()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
        TTF_Init();
        auto fontPath =
            std::string(SDL_GetBasePath()) + p->relFontsPath + "PartyConfettiRegular-eZOn3.ttf";
        p->fonts.insert({FontSizes::_20, std::make_shared<Font>(fontPath.c_str(), 20)});
        p->fonts.insert({FontSizes::_26, std::make_shared<Font>(fontPath.c_str(), 26)});
    }

    RenderId
    TextRenderer::NewString()
    /*/////////////////////*/
    {
        auto uniqueName = p->CreateBlankTexGetName();
        auto rendIdImageRect = Renderer2DImages::Get()->NewImage();
        p->uniqueNameIds.insert({rendIdImageRect, uniqueName});
        return rendIdImageRect;
    }

    RenderId
    TextRenderer::NewBillboardString()
    /*//////////////////////////////*/
    {
        auto uniqueName = p->CreateBlankTexGetName();
        auto rendIdBboardTexRect = RendererBillboardImages::Get()->NewBillboardImage();
        p->uniqueNameIds.insert({rendIdBboardTexRect, uniqueName});
        return rendIdBboardTexRect;
    }

    RenderId
    TextRenderer::NewMultiLineString(int numLines, float width)
    /*///////////////////////////////////////////////////////*/
    {
        MultiLineText multiLineText;
        multiLineText.width = width;
        for (auto i = 0; i < numLines; i++)
            multiLineText.renderIds.push_back(NewString());
        p->multiLines.insert({multiLineText.renderIds.at(0), multiLineText});
        return multiLineText.renderIds.at(0);
    }

    void
    TextRenderer::Pimpl::RenderText(
        RenderId vaoId, std::string_view text, Color color, bool centerAlign, FontSizes textSize,
        std::string &outUniqueNameId, SizeF &outSize) const
    /*/////////////////////////////////////////////////////////////////////////////////////////*/
    {
        auto font = fonts.at(textSize)->GetSdlFont();
        if (!font)
            return;
        auto sdlColor = color.ToSdlColor();
        auto outlineSdlColor = outlineColor.ToSdlColor();
        auto textOutlineSurface = TTF_RenderText_Blended(
            fonts.at(textSize)->GetSdlFontOutline(), text.data(), outlineSdlColor);
        if (!textOutlineSurface)
            return;
        auto textSurface = TTF_RenderText_Blended(font, text.data(), sdlColor);
        glEnable(GL_TEXTURE_2D);
        auto uniqueNameId = uniqueNameIds.at(vaoId);
        auto imageId = ImageBank::Get()->GetImage(uniqueNameId.c_str());
        glBindTexture(GL_TEXTURE_2D, imageId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        auto w = textOutlineSurface->w;
        auto h = textOutlineSurface->h;
        auto image = SDL_CreateRGBSurface(
            SDL_SWSURFACE, w, h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        auto area = SDL_Rect{
            Font::GetFontOutlineWidth(), Font::GetFontOutlineWidth(),
            textSurface ? textSurface->w : 0, textSurface ? textSurface->h : 0};
        auto areaOutline = SDL_Rect{
            0, 0, textOutlineSurface ? textOutlineSurface->w : 0,
            textOutlineSurface ? textOutlineSurface->h : 0};
        SDL_BlitSurface(textOutlineSurface, &area, image, &areaOutline);
        SDL_BlitSurface(textSurface, &areaOutline, image, &areaOutline);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
            image->pixels);
        auto canvasSize = GetCanvasSize();
        auto width = textSurface ? static_cast<float>(textSurface->w) / canvasSize.width : 0;
        auto height = textSurface ? static_cast<float>(textSurface->h) / canvasSize.height : 0;
        outUniqueNameId = uniqueNameId;
        outSize = {width, height};
        SDL_FreeSurface(image);
        SDL_FreeSurface(textSurface);
        SDL_FreeSurface(textOutlineSurface);
    }

    void
    TextRenderer::DrawMultiLineString(
        RenderId glId, const std::string &text, Point2F position, Color color, bool centerAlign,
        FontSizes textSize) const
    /*////////////////////////////////////////////////////////////////////////////////////////*/
    {
        int fullTextW;
        int fullTextH;
        auto multiLineObject = p->multiLines.at(glId);
        auto canvasSize = GetCanvasSize();
        auto aspectRatio = GetAspectRatio();
        TTF_SizeText(p->fonts.at(textSize)->GetSdlFont(), text.c_str(), &fullTextW, &fullTextH);
        auto totalTextWidthF = static_cast<float>(fullTextW) / canvasSize.width;
        auto numLines = totalTextWidthF / multiLineObject.width;
        auto numCharactersPerLine = text.length() / numLines;
        auto lineHeight = static_cast<float>(fullTextH) / canvasSize.height;
        for (auto i = 0; i < numLines; i++)
        /*********************************/
        {
            auto lineText = text.substr(i * numCharactersPerLine, numCharactersPerLine + 1);
            std::string uniqueNameId;
            SizeF size;
            p->RenderText(
                multiLineObject.renderIds.at(i), lineText, color, centerAlign, textSize,
                uniqueNameId, size);
            auto rect =
                RectangleF{position.x, position.y + i * lineHeight, size.width, size.height};
            int textW;
            int textH;
            TTF_SizeText(p->fonts.at(textSize)->GetSdlFont(), text.c_str(), &textW, &textH);
            rect.y -= static_cast<float>(textH / GetAspectRatio()) / canvasSize.height / 2.0f;
            if (centerAlign)
                rect.x -= static_cast<float>(textW) / static_cast<float>(canvasSize.height) / 2.0f /
                          GetAspectRatio();
            Renderer2DImages::Get()->DrawImage(uniqueNameId, multiLineObject.renderIds.at(i), rect);
        }
    }

    void
    TextRenderer::DrawString(
        RenderId vaoId, std::string_view text, Point2F position, Color color, bool centerAlign,
        FontSizes textSize) const
    /*///////////////////////////////////////////////////////////////////////////////////////*/
    {
        std::string uniqueNameId;
        SizeF size;
        p->RenderText(vaoId, text, color, centerAlign, textSize, uniqueNameId, size);
        auto canvasSize = GetCanvasSize();
        auto rect = RectangleF{position.x, position.y, size.width, size.height};
        int textW;
        int textH;
        TTF_SizeText(p->fonts.at(textSize)->GetSdlFont(), text.data(), &textW, &textH);
        rect.y -= static_cast<float>(textH / GetAspectRatio()) / canvasSize.height / 2.0f;
        if (centerAlign)
            rect.x -= static_cast<float>(textW) / static_cast<float>(canvasSize.height) / 2.0f /
                      GetAspectRatio();
        Renderer2DImages::Get()->DrawImage(uniqueNameId, vaoId, rect);
    }

    void
    TextRenderer::DrawBillboardString(
        RenderId vaoId, std::string_view text, Point3F position, SizeF billboardSize, Color color,
        bool centerAlign, FontSizes textSize) const
    /*//////////////////////////////////////////////////////////////////////////////////////////*/
    {
        std::string uniqueNameId;
        SizeF size;
        p->RenderText(vaoId, text, color, centerAlign, textSize, uniqueNameId, size);
        auto canvasSize = GetCanvasSize();
        int text_w;
        int text_h;
        TTF_SizeText(p->fonts.at(textSize)->GetSdlFont(), text.data(), &text_w, &text_h);
        size.height = 1.0f;
        size.width = size.height / static_cast<float>(text_h) * text_w / billboardSize.width *
                     billboardSize.height / GetAspectRatio();
        auto rect = RectangleF{-size.width / 2, -size.height / 2, size.width, size.height};
        rect.y -= static_cast<float>(text_h / GetAspectRatio()) / canvasSize.height / 2.0f;
        if (centerAlign)
            rect.x -= static_cast<float>(text_w) / static_cast<float>(canvasSize.height) / 2.0f /
                      GetAspectRatio();
        RendererBillboardImages::Get()->DrawBillboardImage(
            Hash(uniqueNameId), vaoId, position, rect, billboardSize);
    }

    const std::string
    TextRenderer::Pimpl::CreateBlankTexGetName()
    /*////////////////////////////////////////*/
    {
        auto id = idCounter++;
        auto uniqueName = "RenderedImage" + std::to_string(id);
        ImageBank::Get()->GetBlankTextImage(uniqueName);
        return uniqueName;
    }
}