#include "Renderer2DSolidColors.hpp"
#include "ShaderProgram.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Renderer2DSolidColors::Pimpl
    /*//////////////////////////////*/
    {
      public:
        const int locationPosition = 0;
        const int locationColor = 1;
    };

    Renderer2DSolidColors::Renderer2DSolidColors()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////////////////*/
    {
        const GLchar *vertexShaderSource =
#include "Shaders/2DSolidColorsVertex.glsl"
            ;
        const GLchar *fragmentShaderSource =
#include "Shaders/2DSolidColorsFragment.glsl"
            ;
        GetShaderProgram()->Create(vertexShaderSource, fragmentShaderSource);
    }

    RenderId
    Renderer2DSolidColors::NewRectangle()
    /*/////////////////////////////////*/
    {
        auto vaoId = GenerateNewVertexArrayId();
        UseVaoBegin(vaoId);
        auto indexBufferId = GenerateNewBufferId(BufferTypes::Indices, vaoId);
        auto positionBufferId = GenerateNewBufferId(BufferTypes::Positions, vaoId);
        auto colorBufferId = GenerateNewBufferId(BufferTypes::Colors, vaoId);
        SetIndicesData(indexBufferId, RendererBase::GetNumVerticlesInRectangle(), nullptr);
        SetPositions2DData(positionBufferId, RendererBase::GetNumVerticlesInRectangle(), nullptr);
        SetColorsData(colorBufferId, RendererBase::GetNumVerticlesInRectangle(), nullptr);
        UseVaoEnd();
        return vaoId;
    }

    void
    Renderer2DSolidColors::FillRectangle(RenderId vaoId, const RectangleF &rect, Color Color) const
    /*///////////////////////////////////////////////////////////////////////////////////////////*/
    {
        auto glRect = rect.ToGlRect();
        Vertex2F vertices[RendererBase::kNumVerticesInRectangle];
        vertices[0].position = {glRect.x, glRect.y - glRect.height};
        vertices[1].position = {glRect.x, glRect.y};
        vertices[2].position = {glRect.x + glRect.width, glRect.y};
        vertices[3].position = {glRect.x + glRect.width, glRect.y - glRect.height};
        vertices[0].color = Color;
        vertices[1].color = Color;
        vertices[2].color = Color;
        vertices[3].color = Color;
        glDisable(GL_DEPTH_TEST);
        std::vector<int> indices(RendererBase::GetNumVerticlesInRectangle());
        std::iota(std::begin(indices), std::end(indices), 0);
        std::vector<float> positions;
        std::vector<float> colors;
        for (auto &vertex : vertices)
        /***************************/
        {
            positions.push_back(vertex.position.x);
            positions.push_back(vertex.position.y);
            colors.push_back(vertex.color.r);
            colors.push_back(vertex.color.g);
            colors.push_back(vertex.color.b);
            colors.push_back(vertex.color.a);
        }
        UseVaoBegin(vaoId);
        auto indexBufferId = GetBufferId(BufferTypes::Indices, vaoId);
        auto positionBufferId = GetBufferId(BufferTypes::Positions, vaoId);
        auto colorBufferId = GetBufferId(BufferTypes::Colors, vaoId);
        UpdateIndicesData(indexBufferId, indices);
        UpdatePositions2DData(positionBufferId, positions, p->locationPosition);
        UpdateColorsData(colorBufferId, colors, p->locationColor);
        glDrawElements(
            GL_TRIANGLE_FAN, RendererBase::GetNumVerticlesInRectangle(), GL_UNSIGNED_INT, NULL);
        UseVaoEnd();
    }

    void
    Renderer2DSolidColors::Cleanup()
    /*////////////////////////////*/
    {
        CleanupBase();
    }
}