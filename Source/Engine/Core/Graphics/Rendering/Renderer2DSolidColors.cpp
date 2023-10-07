#include "Renderer2DSolidColors.hpp"
#include "ShaderProgram.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Renderer2DSolidColors::Pimpl
    /*//////////////////////////////*/
    {
      public:
        const int kLocationPosition = 0;
        const int kLocationColor = 1;
    };

    Renderer2DSolidColors::Renderer2DSolidColors()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////////////////*/
    {
        const GLchar *vertex_shader_source =
#include "Shaders/2DSolidColorsVertex.glsl"
            ;
        const GLchar *fragment_shader_source =
#include "Shaders/2DSolidColorsFragment.glsl"
            ;
        GetShaderProgram()->Create(vertex_shader_source, fragment_shader_source);
    }

    RenderId
    Renderer2DSolidColors::NewRectangle()
    /*/////////////////////////////////*/
    {
        auto vao_id = GenerateNewVertexArrayId();
        UseVaoBegin(vao_id);
        auto index_buffer_id = GenerateNewBufferId(BufferTypes::Indices, vao_id);
        auto position_buffer_id = GenerateNewBufferId(BufferTypes::Positions, vao_id);
        auto color_buffer_id = GenerateNewBufferId(BufferTypes::Colors, vao_id);
        SetIndicesData(index_buffer_id, RendererBase::GetNumVerticlesInRectangle(), nullptr);
        SetPositions2DData(position_buffer_id, RendererBase::GetNumVerticlesInRectangle(), nullptr);
        SetColorsData(color_buffer_id, RendererBase::GetNumVerticlesInRectangle(), nullptr);
        UseVaoEnd();
        return vao_id;
    }

    void
    Renderer2DSolidColors::FillRectangle(RenderId vaoId, const RectangleF &rect, Color Color) const
    /*///////////////////////////////////////////////////////////////////////////////////////////*/
    {
        auto gl_rect = rect.ToGlRect();
        Vertex2F vertices[RendererBase::kNumVerticesInRectangle];
        vertices[0].position = {gl_rect.x, gl_rect.y - gl_rect.height};
        vertices[1].position = {gl_rect.x, gl_rect.y};
        vertices[2].position = {gl_rect.x + gl_rect.width, gl_rect.y};
        vertices[3].position = {gl_rect.x + gl_rect.width, gl_rect.y - gl_rect.height};
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
        auto index_buffer_id = GetBufferId(BufferTypes::Indices, vaoId);
        auto position_buffer_id = GetBufferId(BufferTypes::Positions, vaoId);
        auto color_buffer_id = GetBufferId(BufferTypes::Colors, vaoId);
        UpdateIndicesData(index_buffer_id, indices);
        UpdatePositions2DData(position_buffer_id, positions, p->kLocationPosition);
        UpdateColorsData(color_buffer_id, colors, p->kLocationColor);
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