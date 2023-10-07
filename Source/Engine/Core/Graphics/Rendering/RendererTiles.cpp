#include "RendererTiles.hpp"
#include "CameraGl.hpp"
#include "Engine/Assets/ImageBank.hpp"
#include "Engine/Core/Graphics/GraphicsGl.hpp"
#include "ShaderProgram.hpp"
#include "World/Actors/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class RendererTiles::Pimpl
    /*//////////////////////*/
    {
      public:
        const int kLocationPosition = 0;
        const int kLocationColor = 1;
        const int kLocationUv = 2;
        const int kLocationNormal = 3;
        int location_projection_ = -1;
        int location_view_ = -1;
        int location_model_ = -1;
        int location_alpha_ = -1;
        int location_view_pos_ = -1;
        int location_fog_color_ = -1;
        bool is_batch_drawing_ = false;
    };

    RendererTiles::RendererTiles()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
        const GLchar *vertex_shader_source =
#include "Shaders/TilesVertex.glsl"
            ;
        const GLchar *fragment_shader_source =
#include "Shaders/TilesFragment.glsl"
            ;
        GetShaderProgram()->Create(vertex_shader_source, fragment_shader_source);
        p->location_projection_ = GetUniformLocation("projection");
        p->location_view_ = GetUniformLocation("view");
        p->location_model_ = GetUniformLocation("model");
        p->location_alpha_ = GetUniformLocation("mAlpha");
        p->location_view_pos_ = GetUniformLocation("viewPos");
        p->location_fog_color_ = GetUniformLocation("fogColor");
    }

    RenderId
    RendererTiles::NewImagePolygon(int numVertices)
    /*///////////////////////////////////////////*/
    {
        auto vertex_array_id = GenerateNewVertexArrayId();
        auto index_buffer_id = GenerateNewBufferId(BufferTypes::Indices, vertex_array_id);
        auto position_buffer_id = GenerateNewBufferId(BufferTypes::Positions, vertex_array_id);
        auto color_buffer_id = GenerateNewBufferId(BufferTypes::Colors, vertex_array_id);
        auto uv_buffer_id = GenerateNewBufferId(BufferTypes::Uvs, vertex_array_id);
        auto normal_buffer_id = GenerateNewBufferId(BufferTypes::Normals, vertex_array_id);
        glUseProgram(GetShaderProgram()->GetProgramId());
        SetIndicesData(index_buffer_id, numVertices, nullptr);
        SetPositions3DData(position_buffer_id, numVertices, nullptr);
        SetColorsData(color_buffer_id, numVertices, nullptr);
        SetUvsData(uv_buffer_id, numVertices, nullptr);
        SetNormalsData(normal_buffer_id, numVertices, nullptr);
        glBindVertexArray(0);
        glUseProgram(0);
        return vertex_array_id;
    }

    RenderId
    RendererTiles::NewTile()
    /*////////////////////*/
    {
        return NewImagePolygon(4);
    }

    void
    RendererTiles::SetGeometryTile(
        RenderId vaoId, Vertex3F &v0, Vertex3F &v1, Vertex3F &v2, Vertex3F &v3, Point3F &normal00,
        Point3F &normal10, Point3F &normal11, Point3F &normal01) const
    /*//////////////////////////////////////////////////////////////////////////////////////////*/
    {
        std::vector<Vertex3F> vertices;
        v0.normal = normal00;
        v1.normal = normal10;
        v2.normal = normal11;
        v3.normal = normal01;
        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
        SetGeometryImagePolygon(vaoId, vertices);
    }

    void
    RendererTiles::DrawTile(int imageNameHash, RenderId vaoId, bool depthTestOff) const
    /*///////////////////////////////////////////////////////////////////////////////*/
    {
        DrawImagePolygon(imageNameHash, vaoId, 4, depthTestOff);
    }

    void
    RendererTiles::UpdateDrawTile(
        int imageNameHash, RenderId vaoId, Vertex3F &v0, Vertex3F &v1, Vertex3F &v2, Vertex3F &v3,
        Point3F &normal00, Point3F &normal10, Point3F &normal11, Point3F &normal01,
        bool depthTestOff) const
    /*//////////////////////////////////////////////////////////////////////////////////////////*/
    {
        std::vector<Vertex3F> vertices;
        v0.normal = normal00;
        v1.normal = normal10;
        v2.normal = normal11;
        v3.normal = normal01;
        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
        SetGeometryImagePolygon(vaoId, vertices);
        DrawImagePolygon(imageNameHash, vaoId, 4, depthTestOff);
    }

    void
    RendererTiles::SetGeometryImagePolygon(RenderId vaoId, std::vector<Vertex3F> &vertices) const
    /*/////////////////////////////////////////////////////////////////////////////////////////*/
    {
        if (!p->is_batch_drawing_)
            glUseProgram(GetShaderProgram()->GetProgramId());
        std::vector<int> indices(vertices.size());
        std::iota(std::begin(indices), std::end(indices), 0);
        std::vector<float> positions;
        std::vector<float> colors;
        std::vector<float> uvs;
        std::vector<float> normals;
        for (auto &vertex : vertices)
        /***************************/
        {
            positions.push_back(vertex.position.x);
            positions.push_back(vertex.position.y);
            positions.push_back(vertex.position.z);
            colors.push_back(vertex.color.r);
            colors.push_back(vertex.color.g);
            colors.push_back(vertex.color.b);
            colors.push_back(vertex.color.a);
            uvs.push_back(vertex.uv.x);
            uvs.push_back(vertex.uv.y);
            auto vertex_normal = vertex.normal;
            normals.push_back(vertex_normal.x);
            normals.push_back(vertex_normal.y);
            normals.push_back(vertex_normal.z);
        }
        auto index_buffer_id = GetBufferId(BufferTypes::Indices, vaoId);
        auto position_buffer_id = GetBufferId(BufferTypes::Positions, vaoId);
        auto color_buffer_id = GetBufferId(BufferTypes::Colors, vaoId);
        auto uv_buffer_id = GetBufferId(BufferTypes::Uvs, vaoId);
        auto normal_buffer_id = GetBufferId(BufferTypes::Normals, vaoId);
        glBindVertexArray(vaoId);
        UpdateIndicesData(index_buffer_id, indices);
        UpdatePositions3DData(position_buffer_id, positions, p->kLocationPosition);
        UpdateColorsData(color_buffer_id, colors, p->kLocationColor);
        UpdateUvsData(uv_buffer_id, uvs, p->kLocationUv);
        UpdateNormalsData(normal_buffer_id, normals, p->kLocationNormal);
        glBindVertexArray(0);
        if (!p->is_batch_drawing_)
            glUseProgram(0);
    }

    void
    RendererTiles::StartBatchDrawing()
    /*//////////////////////////////*/
    {
        p->is_batch_drawing_ = true;
        glUseProgram(GetShaderProgram()->GetProgramId());
        glUniformMatrix4fv(
            p->location_projection_, 1, GL_FALSE,
            glm::value_ptr(CameraGl::Get()->GetPerspectiveMatrix()));
        glUniformMatrix4fv(
            p->location_view_, 1, GL_FALSE, glm::value_ptr(CameraGl::Get()->GetViewMatrix()));
        glm::mat4 model(1.0);
        glUniformMatrix4fv(p->location_model_, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(p->location_alpha_, 1.0f);
        glm::vec3 view_pos(
            Player::Get()->GetSpaceCoord().x, Player::Get()->GetSpaceCoord().y,
            Player::Get()->GetSpaceCoord().z);
        glUniform3fv(p->location_view_pos_, 1, glm::value_ptr(view_pos));
        glm::vec3 fog_color_gl(
            GraphicsGl::Get()->GetFogColorGround().r, GraphicsGl::Get()->GetFogColorGround().g,
            GraphicsGl::Get()->GetFogColorGround().b);
        glUniform3fv(p->location_fog_color_, 1, glm::value_ptr(fog_color_gl));
        glUseProgram(GetShaderProgram()->GetProgramId());
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    }

    void
    RendererTiles::StopBatchDrawing()
    /*/////////////////////////////*/
    {
        p->is_batch_drawing_ = false;
        glUseProgram(0);
        glDisable(GL_CULL_FACE);
    }

    void
    RendererTiles::DrawImagePolygon(
        int imageNameHash, RenderId vaoId, int vertexCount, bool depthTestOff) const
    /*////////////////////////////////////////////////////////////////////////////*/
    {
        if (depthTestOff)
            glDisable(GL_DEPTH_TEST);
        else
            glEnable(GL_DEPTH_TEST);
        if (!p->is_batch_drawing_)
        /*********************/
        {
            glUseProgram(GetShaderProgram()->GetProgramId());
            glUniformMatrix4fv(
                p->location_projection_, 1, GL_FALSE,
                glm::value_ptr(CameraGl::Get()->GetPerspectiveMatrix()));
            glUniformMatrix4fv(
                p->location_view_, 1, GL_FALSE, glm::value_ptr(CameraGl::Get()->GetViewMatrix()));
            glm::mat4 model(1.0);
            glUniformMatrix4fv(p->location_model_, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1f(p->location_alpha_, 1.0f);
            glm::vec3 view_pos(
                Player::Get()->GetSpaceCoord().x, Player::Get()->GetSpaceCoord().y,
                Player::Get()->GetSpaceCoord().z);
            glUniform3fv(p->location_view_pos_, 1, glm::value_ptr(view_pos));
            glm::vec3 fog_color_gl(
                GraphicsGl::Get()->GetFogColorGround().r, GraphicsGl::Get()->GetFogColorGround().g,
                GraphicsGl::Get()->GetFogColorGround().b);
            glUniform3fv(p->location_fog_color_, 1, glm::value_ptr(fog_color_gl));
            glUseProgram(GetShaderProgram()->GetProgramId());
        }
        auto imageId = ImageBank::Get()->GetImage(imageNameHash);
        glBindTexture(GL_TEXTURE_2D, imageId);
        glBindVertexArray(vaoId);
        glDrawElements(GL_TRIANGLE_FAN, vertexCount, GL_UNSIGNED_INT, NULL);
        glBindVertexArray(0);
        if (!p->is_batch_drawing_)
            glUseProgram(0);
    }

    void
    RendererTiles::Cleanup()
    /*////////////////////*/
    {
        CleanupBase();
    }
}