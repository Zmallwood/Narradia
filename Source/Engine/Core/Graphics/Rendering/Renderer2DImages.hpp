//////////////////////////////////////////////////////////////////////
#pragma once
#include "RendererBase.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Renderer2DImages : RendererBase,
                             public Singleton<Renderer2DImages>
    /*/////////////////////////////////////////////////////////////////*/
    {
      public:
        Renderer2DImages();
        RenderId NewImage();
        void DrawImage(
            const std::string_view &, RenderId, const RectangleF &, Color = Colors::white) const;
        void DrawImage(int, RenderId, const RectangleF &, Color = Colors::white) const;
        void Cleanup();
        template <int N>
        auto NewImages()
        /*/////////////////////////////*/
        {
            std::array<RenderId, N> result;
            for (auto &entry : result)
                entry = NewImage();
            return std::tuple_cat(result);
        }

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}
//////////////////////////////////////////////////////////////////////