#pragma once
#include "RendererBase.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class RendererTiles : public RendererBase, public Singleton<RendererTiles> {
      public:
        RendererTiles();
        RenderId NewTile();
        void SetGeometryTile(
            RenderId, Vertex3F &, Vertex3F &, Vertex3F &, Vertex3F &, Point3F &, Point3F &,
            Point3F &, Point3F &) const;
        void DrawTile(int, RenderId, bool = false) const;
        void UpdateDrawTile(
            int, RenderId, Vertex3F &, Vertex3F &, Vertex3F &, Vertex3F &, Point3F &, Point3F &,
            Point3F &, Point3F &, bool = false) const;
        void StartBatchDrawing();
        void StopBatchDrawing();
        void Cleanup();

      private:
        void DrawImagePolygon(int, RenderId, int, bool = false) const;
        RenderId NewImagePolygon(int);
        void SetGeometryImagePolygon(RenderId, std::vector<Vertex3F> &) const;

        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}