//////////////////////////////////////////////////////////////////////
#pragma once
#include "RendererBase.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class RendererTiles : public RendererBase,
                          public Singleton<RendererTiles>
    /*//////////////////////////////////////////////////////////////////*/
    {
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

    enum class TextSizes
    /*////////////////*/
    {
        _20,
        _26
    };

    class Font
    /*//////*/
    {
      public:
        Font(std::string_view fontFileName, int fontSize);
        TTF_Font *GetSdlFont() const;
        TTF_Font *GetSdlFontOutline() const;
        static constexpr int GetFontOutlineWidth();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    struct MultiLineText
    /*////////////////*/
    {
        std::vector<RenderId> renderIds;
        float width;
    };

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
            TextSizes = TextSizes::_20) const;
        void DrawBillboardString(
            RenderId, std::string_view, Point3F, SizeF, Color = Colors::wheat, bool = false,
            TextSizes = TextSizes::_20) const;
        void DrawMultiLineString(
            RenderId, const std::string &, Point2F, Color = Colors::wheat, bool = false,
            TextSizes = TextSizes::_20) const;
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