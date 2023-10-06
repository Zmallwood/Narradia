//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    enum class BufferTypes
    /*//////////////////*/
    {
        Indices,
        Positions,
        Colors,
        Uvs,
        Normals
    };

    class SdlDeleter
    /*////////////*/
    {
      public:
        void operator()(SDL_Window *) const;
        void operator()(SDL_Renderer *) const;
        void operator()(SDL_Surface *) const;
        void operator()(SDL_Texture *) const;
        void operator()(TTF_Font *) const;
    };

    class ShaderProgram
    /*///////////////*/
    {
      public:
        ShaderProgram();
        bool Create(const GLchar *, const GLchar *);
        void Cleanup() const;
        GLuint GetProgramId();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class RndrBase
    /*//////////*/
    {
      public:
        RndrBase();

      protected:
        GLuint GenerateNewVertexArrayId();
        GLuint GenerateNewBufferId(BufferTypes, GLuint);
        GLuint GetBufferId(BufferTypes, GLuint) const;
        void CleanupBase() const;

      protected:
        void SetIndicesData(GLuint, int, const void *) const;
        void SetPositions2DData(GLuint, int, const void *, int = -1) const;
        void SetPositions3DData(GLuint, int, const void *, int = -1) const;
        void SetUvsData(GLuint, int, const void *, int = -1) const;
        void SetColorsData(GLuint, int, const void *, int = -1) const;
        void SetNormalsData(GLuint, int, const void *, int = -1) const;
        void UpdateIndicesData(GLuint, std::vector<int> &) const;
        void UpdatePositions2DData(GLuint, std::vector<float> &, int) const;
        void UpdatePositions3DData(GLuint, std::vector<float> &, int) const;
        void UpdateUvsData(GLuint, std::vector<float> &, int) const;
        void UpdateColorsData(GLuint, std::vector<float> &, int) const;
        void UpdateNormalsData(GLuint, std::vector<float> &, int) const;
        void UseVaoBegin(int) const;
        void UseVaoEnd() const;
        GLuint GetUniformLocation(std::string_view);
        ShaderProgram *GetShaderProgram() const;
        static const int GetNumVerticlesInRectangle();
        static constexpr int kNumVerticesInRectangle = 4;

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class CameraGl : public Singleton<CameraGl>
    /*///////////////////////////////////////*/
    {
      public:
        CameraGl();
        const glm::mat4 &GetPerspectiveMatrix();
        const glm::mat4 &GetViewMatrix();
        void SetPerspectiveMatrix(glm::mat4);
        void SetViewMatrix(glm::mat4);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class Renderer2DImages : RndrBase,
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
        /*/////////////////////////////*/ {
            std::array<RenderId, N> result;
            for (auto &entry : result)
                entry = NewImage();
            return std::tuple_cat(result);
        }

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class Renderer2DSolidColors : public RndrBase,
                                  public Singleton<Renderer2DSolidColors>
    /*//////////////////////////////////////////////////////////////////////////////////*/
    {
      public:
        Renderer2DSolidColors();
        RenderId NewRectangle();
        void FillRectangle(RenderId, const RectangleF &, Color) const;
        void Cleanup();
        template <int N>
        auto NewRectangles()
        /*/////////////////////////////////*/ {
            std::array<RenderId, N> result;
            for (auto &entry : result)
                entry = NewRectangle();
            return std::tuple_cat(result);
        }

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class RendererBillboardImages : public RndrBase,
                                    public Singleton<RendererBillboardImages>
    /*//////////////////////////////////////////////////////////////////////////////////////*/
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

    class RendererModels : public RndrBase,
                           public Singleton<RendererModels>
    /*////////////////////////////////////////////////////////////////////*/
    {
      public:
        RendererModels();
        void NewModel(int);
        void DrawModel(
            int, float, Point3F, float = 0.0f, float = 1.0f, float = 1.0f,
            glm::vec3 = {1.0f, 1.0f, 1.0f}, bool = false, bool = false) const;
        void DrawModelsMany(
            int, float, std::vector<Point3F>, std::vector<float>, std::vector<float>,
            std::vector<float>, std::vector<glm::vec3>) const;
        void StartBatchDrawing();
        void StopBatchDrawing();
        void Cleanup();

      private:
        RenderId NewBodyKeyframe(int, float, int);
        void
        NewBodyKeyframeGeometry(int, float, RenderId, std::vector<Vertex3F>, std::vector<Point3F>);

        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class RendererTiles : public RndrBase,
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