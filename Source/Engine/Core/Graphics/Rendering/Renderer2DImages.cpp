#include "Renderer2DImages.hpp"
#include "Engine/Assets/ImageBank.hpp"
#include "ShaderProgram.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Renderer2DImages::Pimpl
    /*/////////////////////////*/
    {
      public:
        const int kLocationPosition = 0;
        const int kLocationColor = 1;
        const int kLocationUv = 2;
    };

    Renderer2DImages::Renderer2DImages()
        : p(std::make_shared<Pimpl>())
    /*////////////////////////////////*/
    {
        const GLchar *vertex_shader_source =
#include "Shaders/2DImagesVertex.glsl"
            ;
        const GLchar *fragment_shader_source =
#include "Shaders/2DImagesFragment.glsl"
            ;
        GetShaderProgram()->Create(vertex_shader_source, fragment_shader_source);
    }

    RenderId
    Renderer2DImages::NewImage()
    /*////////////////////////*/
    {
        auto vao_id = GenerateNewVertexArrayId();
        UseVaoBegin(vao_id);
        auto index_buffer_id = GenerateNewBufferId(BufferTypes::Indices, vao_id);
        auto position_buffer_id = GenerateNewBufferId(BufferTypes::Positions, vao_id);
        auto color_buffer_id = GenerateNewBufferId(BufferTypes::Colors, vao_id);
        auto uv_buffer_id = GenerateNewBufferId(BufferTypes::Uvs, vao_id);
        SetIndicesData(index_buffer_id, RendererBase::GetNumVerticlesInRectangle(), nullptr);
        SetPositions2DData(position_buffer_id, RendererBase::GetNumVerticlesInRectangle(), nullptr);
        SetColorsData(color_buffer_id, RendererBase::GetNumVerticlesInRectangle(), nullptr);
        SetUvsData(uv_buffer_id, RendererBase::GetNumVerticlesInRectangle(), nullptr);
        UseVaoEnd();
        return vao_id;
    }

    void
    Renderer2DImages::DrawImage(
        int imageNameHash, RenderId vaoId, const RectangleF &rectangle, Color color) const
    /*//////////////////////////////////////////////////////////////////////////////////*/
    {
        auto gl_rect = rectangle.ToGlRect();
        Vertex2F vertices[RendererBase::kNumVerticesInRectangle];
        vertices[0].position = {gl_rect.x, gl_rect.y - gl_rect.height};
        vertices[1].position = {gl_rect.x, gl_rect.y};
        vertices[2].position = {gl_rect.x + gl_rect.width, gl_rect.y};
        vertices[3].position = {gl_rect.x + gl_rect.width, gl_rect.y - gl_rect.height};
        vertices[0].color = color;
        vertices[1].color = color;
        vertices[2].color = color;
        vertices[3].color = color;
        vertices[0].uv = {0.0f, 1.0f};
        vertices[1].uv = {0.0f, 0.0f};
        vertices[2].uv = {1.0f, 0.0f};
        vertices[3].uv = {1.0f, 1.0f};
        glDisable(GL_DEPTH_TEST);
        auto image_id = ImageBank::Get()->GetImage(imageNameHash);
        glBindTexture(GL_TEXTURE_2D, image_id);
        std::vector<int> indices(RendererBase::GetNumVerticlesInRectangle());
        std::iota(std::begin(indices), std::end(indices), 0);
        std::vector<float> positions;
        std::vector<float> colors;
        std::vector<float> uvs;
        for (auto &vertex : vertices)
        /***************************/
        {
            positions.push_back(vertex.position.x);
            positions.push_back(vertex.position.y);
            colors.push_back(vertex.color.r);
            colors.push_back(vertex.color.g);
            colors.push_back(vertex.color.b);
            colors.push_back(vertex.color.a);
            uvs.push_back(vertex.uv.x);
            uvs.push_back(vertex.uv.y);
        }
        UseVaoBegin(vaoId);
        auto index_buffer_id = GetBufferId(BufferTypes::Indices, vaoId);
        auto position_buffer_id = GetBufferId(BufferTypes::Positions, vaoId);
        auto color_buffer_id = GetBufferId(BufferTypes::Colors, vaoId);
        auto uv_buffer_id = GetBufferId(BufferTypes::Uvs, vaoId);
        UpdateIndicesData(index_buffer_id, indices);
        UpdatePositions2DData(position_buffer_id, positions, p->kLocationPosition);
        UpdateColorsData(color_buffer_id, colors, p->kLocationColor);
        UpdateUvsData(uv_buffer_id, uvs, p->kLocationUv);
        glDrawElements(
            GL_TRIANGLE_FAN, RendererBase::GetNumVerticlesInRectangle(), GL_UNSIGNED_INT, NULL);
        UseVaoEnd();
    }

    void
    Renderer2DImages::DrawImage(
        const std::string_view &imageName, RenderId vaoId, const RectangleF &rectangle,
        Color color) const
    /*///////////////////////////////////////////////////////////////////////////////*/
    {
        DrawImage(Hash(imageName), vaoId, rectangle, color);
    }

    void
    Renderer2DImages::Cleanup()
    /*///////////////////////*/
    {
        CleanupBase();
    }
}