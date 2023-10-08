#include "TextRenderer.hpp"
#include "Engine/Assets/ImageBank.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/RendererBillboardImages.hpp"
#include "Font.hpp"
#include "MultiLineText.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class TextRenderer::Pimpl {
      public:
        void RenderText(
            RenderId, std::string_view, Color, bool, FontSizes, std::string &, SizeF &) const;
        const std::string CreateBlankTexGetName();

        const std::string kRelFontsPath = "Resources/Fonts/";
        const Color outline_color_ = Colors::black;
        std::map<FontSizes, std::shared_ptr<const Font>> fonts_;
        std::map<RenderId, std::string> unique_name_ids_;
        int id_counter_ = 0;
        std::map<RenderId, MultiLineText> multi_lines_;
    };

    TextRenderer::TextRenderer()
        : p(std::make_shared<Pimpl>()) {
        TTF_Init();
        auto font_path =
            std::string(SDL_GetBasePath()) + p->kRelFontsPath + "PartyConfettiRegular-eZOn3.ttf";
        p->fonts_.insert({FontSizes::_20, std::make_shared<Font>(font_path.c_str(), 20)});
        p->fonts_.insert({FontSizes::_26, std::make_shared<Font>(font_path.c_str(), 26)});
    }

    RenderId TextRenderer::NewString() {
        auto unique_name = p->CreateBlankTexGetName();
        auto rendid_image_rect = Renderer2DImages::Get()->NewImage();
        p->unique_name_ids_.insert({rendid_image_rect, unique_name});
        return rendid_image_rect;
    }

    RenderId TextRenderer::NewBillboardString() {
        auto unique_name = p->CreateBlankTexGetName();
        auto rendid_billboard_tex_rect = RendererBillboardImages::Get()->NewBillboardImage();
        p->unique_name_ids_.insert({rendid_billboard_tex_rect, unique_name});
        return rendid_billboard_tex_rect;
    }

    RenderId TextRenderer::NewMultiLineString(int numLines, float width) {
        MultiLineText multi_line_text;
        multi_line_text.width = width;
        for (auto i = 0; i < numLines; i++)
            multi_line_text.renderIds.push_back(NewString());
        p->multi_lines_.insert({multi_line_text.renderIds.at(0), multi_line_text});
        return multi_line_text.renderIds.at(0);
    }

    void TextRenderer::Pimpl::RenderText(
        RenderId vaoId, std::string_view text, Color color, bool centerAlign, FontSizes textSize,
        std::string &outUniqueNameId, SizeF &outSize) const {
        auto font = fonts_.at(textSize)->GetSdlFont();
        if (!font)
            return;
        auto sdl_color = color.ToSdlColor();
        auto outline_sdl_color = outline_color_.ToSdlColor();
        auto text_outline_surface = TTF_RenderText_Blended(
            fonts_.at(textSize)->GetSdlFontOutline(), text.data(), outline_sdl_color);
        if (!text_outline_surface)
            return;
        auto text_surface = TTF_RenderText_Blended(font, text.data(), sdl_color);
        glEnable(GL_TEXTURE_2D);
        auto unique_name_id = unique_name_ids_.at(vaoId);
        auto image_id = ImageBank::Get()->GetImage(unique_name_id.c_str());
        glBindTexture(GL_TEXTURE_2D, image_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        auto w = text_outline_surface->w;
        auto h = text_outline_surface->h;
        auto image = SDL_CreateRGBSurface(
            SDL_SWSURFACE, w, h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        auto area = SDL_Rect{
            Font::GetFontOutlineWidth(), Font::GetFontOutlineWidth(),
            text_surface ? text_surface->w : 0, text_surface ? text_surface->h : 0};
        auto area_outline = SDL_Rect{
            0, 0, text_outline_surface ? text_outline_surface->w : 0,
            text_outline_surface ? text_outline_surface->h : 0};
        SDL_BlitSurface(text_outline_surface, &area, image, &area_outline);
        SDL_BlitSurface(text_surface, &area_outline, image, &area_outline);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
            image->pixels);
        auto canvas_size = GetCanvasSize();
        auto width = text_surface ? static_cast<float>(text_surface->w) / canvas_size.width : 0;
        auto height = text_surface ? static_cast<float>(text_surface->h) / canvas_size.height : 0;
        outUniqueNameId = unique_name_id;
        outSize = {width, height};
        SDL_FreeSurface(image);
        SDL_FreeSurface(text_surface);
        SDL_FreeSurface(text_outline_surface);
    }

    void TextRenderer::DrawMultiLineString(
        RenderId glId, const std::string &text, Point2F position, Color color, bool centerAlign,
        FontSizes textSize) const {
        int full_text_w;
        int full_text_h;
        auto multi_line_object = p->multi_lines_.at(glId);
        auto canvas_size = GetCanvasSize();
        auto aspect_ratio = GetAspectRatio();
        TTF_SizeText(
            p->fonts_.at(textSize)->GetSdlFont(), text.c_str(), &full_text_w, &full_text_h);
        auto total_text_width_f = static_cast<float>(full_text_w) / canvas_size.width;
        auto num_lines = total_text_width_f / multi_line_object.width;
        auto num_characters_per_line = text.length() / num_lines;
        auto line_height = static_cast<float>(full_text_h) / canvas_size.height;
        for (auto i = 0; i < num_lines; i++) {
            auto line_text = text.substr(i * num_characters_per_line, num_characters_per_line + 1);
            std::string unique_name_id;
            SizeF size;
            p->RenderText(
                multi_line_object.renderIds.at(i), line_text, color, centerAlign, textSize,
                unique_name_id, size);
            auto rect =
                RectangleF{position.x, position.y + i * line_height, size.width, size.height};
            int text_w;
            int text_h;
            TTF_SizeText(p->fonts_.at(textSize)->GetSdlFont(), text.c_str(), &text_w, &text_h);
            rect.y -= static_cast<float>(text_h / GetAspectRatio()) / canvas_size.height / 2.0f;
            if (centerAlign)
                rect.x -= static_cast<float>(text_w) / static_cast<float>(canvas_size.height) /
                          2.0f / GetAspectRatio();
            Renderer2DImages::Get()->DrawImage(
                unique_name_id, multi_line_object.renderIds.at(i), rect);
        }
    }

    void TextRenderer::DrawString(
        RenderId vaoId, std::string_view text, Point2F position, Color color, bool centerAlign,
        FontSizes textSize) const {
        std::string unique_name_id;
        SizeF size;
        p->RenderText(vaoId, text, color, centerAlign, textSize, unique_name_id, size);
        auto canvas_size = GetCanvasSize();
        auto rect = RectangleF{position.x, position.y, size.width, size.height};
        int text_w;
        int text_h;
        TTF_SizeText(p->fonts_.at(textSize)->GetSdlFont(), text.data(), &text_w, &text_h);
        rect.y -= static_cast<float>(text_h / GetAspectRatio()) / canvas_size.height / 2.0f;
        if (centerAlign)
            rect.x -= static_cast<float>(text_w) / static_cast<float>(canvas_size.height) / 2.0f /
                      GetAspectRatio();
        Renderer2DImages::Get()->DrawImage(unique_name_id, vaoId, rect);
    }

    void TextRenderer::DrawBillboardString(
        RenderId vaoId, std::string_view text, Point3F position, SizeF billboardSize, Color color,
        bool centerAlign, FontSizes textSize) const {
        std::string unique_name_id;
        SizeF size;
        p->RenderText(vaoId, text, color, centerAlign, textSize, unique_name_id, size);
        auto canvas_size = GetCanvasSize();
        int text_w;
        int text_h;
        TTF_SizeText(p->fonts_.at(textSize)->GetSdlFont(), text.data(), &text_w, &text_h);
        size.height = 1.0f;
        size.width = size.height / static_cast<float>(text_h) * text_w / billboardSize.width *
                     billboardSize.height / GetAspectRatio();
        auto rect = RectangleF{-size.width / 2, -size.height / 2, size.width, size.height};
        rect.y -= static_cast<float>(text_h / GetAspectRatio()) / canvas_size.height / 2.0f;
        if (centerAlign)
            rect.x -= static_cast<float>(text_w) / static_cast<float>(canvas_size.height) / 2.0f /
                      GetAspectRatio();
        RendererBillboardImages::Get()->DrawBillboardImage(
            Hash(unique_name_id), vaoId, position, rect, billboardSize);
    }

    const std::string TextRenderer::Pimpl::CreateBlankTexGetName() {
        auto id = id_counter_++;
        auto unique_name = "RenderedImage" + std::to_string(id);
        ImageBank::Get()->GetBlankTextImage(unique_name);
        return unique_name;
    }
}