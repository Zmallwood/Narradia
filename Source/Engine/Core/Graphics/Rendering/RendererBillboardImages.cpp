#include "RendererBillboardImages.hpp"
#include "CameraGl.hpp"
#include "Engine/Assets/ImageBank.hpp"
#include "ShaderProgram.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class RendererBillboardImages::Pimpl {
      public:
        const int kLocationPosition = 0;
        const int kLocationUv = 1;
        int location_projection_ = -1;
        int location_view_ = -1;
        int location_particle_center_worldspace_ = -1;
        int location_billboard_size_ = -1;
    };

    RendererBillboardImages::RendererBillboardImages()
        : p(std::make_shared<Pimpl>()) {
        const GLchar *vertex_shader_source =
#include "Shaders/BillboardImagesVertex.glsl"
            ;
        const GLchar *fragment_shader_source =
#include "Shaders/BillboardImagesFragment.glsl"
            ;
        GetShaderProgram()->Create(vertex_shader_source, fragment_shader_source);
        p->location_projection_ = GetUniformLocation("projection");
        p->location_view_ = GetUniformLocation("view");
        p->location_particle_center_worldspace_ = GetUniformLocation("particleCenterWorldspace");
        p->location_billboard_size_ = GetUniformLocation("billboardSize");
    }

    RenderId RendererBillboardImages::NewBillboardImage() {
        auto vao_id = GenerateNewVertexArrayId();
        UseVaoBegin(vao_id);
        auto index_buffer_id = GenerateNewBufferId(BufferTypes::Indices, vao_id);
        auto position_buffer_id = GenerateNewBufferId(BufferTypes::Positions, vao_id);
        auto uv_buffer_id = GenerateNewBufferId(BufferTypes::Uvs, vao_id);
        const auto numVertices = RendererBase::GetNumVerticlesInRectangle();
        SetIndicesData(index_buffer_id, numVertices, nullptr);
        SetPositions2DData(position_buffer_id, numVertices, nullptr);
        SetUvsData(uv_buffer_id, numVertices, nullptr);
        UseVaoEnd();
        return vao_id;
    }

    void RendererBillboardImages::DrawBillboardImage(
        int imageNameHash, RenderId vaoId, Point3F position, RectangleF &bounds,
        SizeF billboardSize) const {
        Vertex3F vertices[RendererBase::kNumVerticesInRectangle];
        vertices[0].uv = {0, 0};
        vertices[1].uv = {1, 0};
        vertices[2].uv = {1, 1};
        vertices[3].uv = {0, 1};
        auto gl_rect = bounds.InvertVertically();
        vertices[0].position = {gl_rect.x, gl_rect.y};
        vertices[1].position = {gl_rect.x + gl_rect.width, gl_rect.y};
        vertices[2].position = {gl_rect.x + gl_rect.width, gl_rect.y + gl_rect.height};
        vertices[3].position = {gl_rect.x, gl_rect.y + gl_rect.height};
        glEnable(GL_DEPTH_TEST);
        UseVaoBegin(vaoId);
        glUniformMatrix4fv(
            p->location_projection_, 1, GL_FALSE,
            glm::value_ptr(CameraGl::Get()->GetPerspectiveMatrix()));
        glUniformMatrix4fv(
            p->location_view_, 1, GL_FALSE, glm::value_ptr(CameraGl::Get()->GetViewMatrix()));
        glm::vec3 center_gl = {position.x, position.y, position.z};
        glUniform3fv(p->location_particle_center_worldspace_, 1, &center_gl[0]);
        glm::vec2 glm_billboard_size = {billboardSize.width, billboardSize.height};
        glUniform2fv(p->location_billboard_size_, 1, &glm_billboard_size[0]);
        auto image_id = ImageBank::Get()->GetImage(imageNameHash);
        glBindTexture(GL_TEXTURE_2D, image_id);
        std::vector<int> indices(RendererBase::GetNumVerticlesInRectangle());
        std::iota(std::begin(indices), std::end(indices), 0);
        std::vector<float> positions;
        std::vector<float> uvs;
        for (auto &v : vertices) {
            positions.push_back(v.position.x);
            positions.push_back(v.position.y);
            uvs.push_back(v.uv.x);
            uvs.push_back(v.uv.y);
        }
        auto index_buffer_id = GetBufferId(BufferTypes::Indices, vaoId);
        auto position_buffer_id = GetBufferId(BufferTypes::Positions, vaoId);
        auto uv_buffer_id = GetBufferId(BufferTypes::Uvs, vaoId);
        UpdateIndicesData(index_buffer_id, indices);
        UpdatePositions2DData(position_buffer_id, positions, p->kLocationPosition);
        UpdateUvsData(uv_buffer_id, uvs, p->kLocationUv);
        glDrawElements(
            GL_TRIANGLE_FAN, RendererBase::GetNumVerticlesInRectangle(), GL_UNSIGNED_INT, NULL);
        UseVaoEnd();
    }

    void RendererBillboardImages::Cleanup() {
        CleanupBase();
    }
}