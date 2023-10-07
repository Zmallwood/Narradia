//////////////////////////////////////////////////////////////////////
#pragma once
#include "BufferTypes.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ShaderProgram;

    class RendererBase
    /*//////////*/
    {
      public:
        RendererBase();

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
}