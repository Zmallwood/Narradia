//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Font
    /*//////*/
    {
      public:
        Font(std::string_view fontFileName, int fontSize);
        TTF_Font *GetSdlFont() const;
        TTF_Font *GetSdlFontOutline() const;
        inline static constexpr int GetFontOutlineWidth()
        /*/////////////////////////////////////////////*/
        {
            return kFontOutlineWidth;
        }

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;

        inline static constexpr int kFontOutlineWidth = 2;
    };
}
//////////////////////////////////////////////////////////////////////