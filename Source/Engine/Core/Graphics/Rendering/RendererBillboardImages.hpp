#pragma once
#include "RendererBase.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class RendererBillboardImages : public RendererBase,
                                    public Singleton<RendererBillboardImages>
    /*/////////////////////////////////////////////////////////////////////*/
    {
      public:
        RendererBillboardImages();
        RenderId NewBillboardImage();
        void DrawBillboardImage(int, RenderId, Point3F, RectangleF &, SizeF) const;
        void Cleanup();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
} 