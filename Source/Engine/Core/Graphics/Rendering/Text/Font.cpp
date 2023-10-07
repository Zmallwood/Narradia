#include "Font.hpp"
#include "Engine/SdlDeleter.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Font::Pimpl
    /*/////////////*/
    {
      public:
        std::shared_ptr<TTF_Font> sdl_font_ = nullptr;
        std::shared_ptr<TTF_Font> sdl_font_outline_ = nullptr;
    };

    Font::Font(std::string_view fontFileName, int fontSize)
        : p(std::make_shared<Pimpl>())
    /*///////////////////////////////////////////////////*/
    {
        p->sdl_font_ =
            std::shared_ptr<TTF_Font>(TTF_OpenFont(fontFileName.data(), fontSize), SdlDeleter());
        p->sdl_font_outline_ =
            std::shared_ptr<TTF_Font>(TTF_OpenFont(fontFileName.data(), fontSize), SdlDeleter());
        TTF_SetFontOutline(p->sdl_font_outline_.get(), kFontOutlineWidth);
    }

    TTF_Font *
    Font::GetSdlFont() const
    /*////////////////////*/
    {
        return p->sdl_font_.get();
    }

    TTF_Font *
    Font::GetSdlFontOutline() const
    /*///////////////////////////*/
    {
        return p->sdl_font_outline_.get();
    }
}