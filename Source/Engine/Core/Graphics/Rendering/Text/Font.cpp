//////////////////////////////////////////////////////////////////////
#include "Font.hpp"
#include "Engine/SdlDeleter.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Font::Pimpl
    /*/////////////*/
    {
      public:
        std::shared_ptr<TTF_Font> sdlFont = nullptr;
        std::shared_ptr<TTF_Font> sdlFontOutline = nullptr;
    };

    Font::Font(std::string_view fontFileName, int fontSize)
        : p(std::make_shared<Pimpl>())
    /*///////////////////////////////////////////////////*/
    {
        p->sdlFont =
            std::shared_ptr<TTF_Font>(TTF_OpenFont(fontFileName.data(), fontSize), SdlDeleter());
        p->sdlFontOutline =
            std::shared_ptr<TTF_Font>(TTF_OpenFont(fontFileName.data(), fontSize), SdlDeleter());
        TTF_SetFontOutline(p->sdlFontOutline.get(), kFontOutlineWidth);
    }

    TTF_Font *
    Font::GetSdlFont() const
    /*////////////////////*/
    {
        return p->sdlFont.get();
    }

    TTF_Font *
    Font::GetSdlFontOutline() const
    /*///////////////////////////*/
    {
        return p->sdlFontOutline.get();
    }
}
//////////////////////////////////////////////////////////////////////