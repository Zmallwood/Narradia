//////////////////////////////////////////////////////////////////////
#pragma once
#include "RendererBase.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Renderer2DSolidColors : public RendererBase,
                                  public Singleton<Renderer2DSolidColors>
    /*/////////////////////////////////////////////////////////////////*/
    {
      public:
        Renderer2DSolidColors();
        RenderId NewRectangle();
        void FillRectangle(RenderId, const RectangleF &, Color) const;
        void Cleanup();
        template <int N>
        auto NewRectangles()
        /*////////////////*/
        {
            std::array<RenderId, N> result;
            for (auto &entry : result)
                entry = NewRectangle();
            return std::tuple_cat(result);
        }

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}
//////////////////////////////////////////////////////////////////////