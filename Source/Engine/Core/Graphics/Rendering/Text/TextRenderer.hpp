//////////////////////////////////////////////////////////////////////
#pragma once
#include "FontSizes.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class TextRenderer : public Singleton<TextRenderer>
    /*///////////////////////////////////////////////*/
    {
      public:
        TextRenderer();
        RenderId NewString();
        RenderId NewBillboardString();
        RenderId NewMultiLineString(int, float);
        void DrawString(
            RenderId, std::string_view, Point2F, Color = Colors::wheat, bool = false,
            FontSizes = FontSizes::_20) const;
        void DrawBillboardString(
            RenderId, std::string_view, Point3F, SizeF, Color = Colors::wheat, bool = false,
            FontSizes = FontSizes::_20) const;
        void DrawMultiLineString(
            RenderId, const std::string &, Point2F, Color = Colors::wheat, bool = false,
            FontSizes = FontSizes::_20) const;
        template <int N>
        auto NewStrings()
        /*/////////////*/
        {
            std::array<RenderId, N> result;
            for (auto &entry : result)
                entry = NewString();
            return std::tuple_cat(result);
        }

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}
//////////////////////////////////////////////////////////////////////