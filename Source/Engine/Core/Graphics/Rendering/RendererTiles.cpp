//////////////////////////////////////////////////////////////////////
#include "RendererTiles.hpp"
#include "CameraGl.hpp"
#include "Engine/Core/Graphics/GraphicsGl.hpp"
#include "ShaderProgram.hpp"
#include "World/Player.hpp"
#include "Engine/Assets/ImageBank.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class RendererTiles::Pimpl
    /*//////////////////////*/
    {
      public:
        const int locationPosition = 0;
        const int locationColor = 1;
        const int locationUv = 2;
        const int locationNormal = 3;
        int locationProjection = -1;
        int locationView = -1;
        int locationModel = -1;
        int locationAlpha = -1;
        int locationViewPos = -1;
        int locationFogColor = -1;
        bool isBatchDrawing = false;
    };

    RendererTiles::RendererTiles()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
        const GLchar *vertexShaderSource =
#include "Shaders/TilesVertex.glsl"
            ;
        const GLchar *fragmentShaderSource =
#include "Shaders/TilesFragment.glsl"
            ;
        GetShaderProgram()->Create(vertexShaderSource, fragmentShaderSource);
        p->locationProjection = GetUniformLocation("projection");
        p->locationView = GetUniformLocation("view");
        p->locationModel = GetUniformLocation("model");
        p->locationAlpha = GetUniformLocation("mAlpha");
        p->locationViewPos = GetUniformLocation("viewPos");
        p->locationFogColor = GetUniformLocation("fogColor");
    }

    RenderId RendererTiles::NewImagePolygon(int numVertices)
    /*////////////////////////////////////////////////////*/
    {
        auto vertexArrayId = GenerateNewVertexArrayId();
        auto indexBufferId = GenerateNewBufferId(BufferTypes::Indices, vertexArrayId);
        auto positionBufferId = GenerateNewBufferId(BufferTypes::Positions, vertexArrayId);
        auto colorBufferId = GenerateNewBufferId(BufferTypes::Colors, vertexArrayId);
        auto uvBufferId = GenerateNewBufferId(BufferTypes::Uvs, vertexArrayId);
        auto normalBufferId = GenerateNewBufferId(BufferTypes::Normals, vertexArrayId);
        glUseProgram(GetShaderProgram()->GetProgramId());
        SetIndicesData(indexBufferId, numVertices, nullptr);
        SetPositions3DData(positionBufferId, numVertices, nullptr);
        SetColorsData(colorBufferId, numVertices, nullptr);
        SetUvsData(uvBufferId, numVertices, nullptr);
        SetNormalsData(normalBufferId, numVertices, nullptr);
        glBindVertexArray(0);
        glUseProgram(0);
        return vertexArrayId;
    }

    RenderId RendererTiles::NewTile()
    /*/////////////////////////////*/
    {
        return NewImagePolygon(4);
    }

    void RendererTiles::SetGeometryTile(
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

    void RendererTiles::DrawTile(int imageNameHash, RenderId vaoId, bool depthTestOff) const
    /*////////////////////////////////////////////////////////////////////////////////////*/
    {
        DrawImagePolygon(imageNameHash, vaoId, 4, depthTestOff);
    }

    void RendererTiles::UpdateDrawTile(
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
        if (!p->isBatchDrawing)
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
        auto indexBufferId = GetBufferId(BufferTypes::Indices, vaoId);
        auto positionBufferId = GetBufferId(BufferTypes::Positions, vaoId);
        auto colorBufferId = GetBufferId(BufferTypes::Colors, vaoId);
        auto uvBufferId = GetBufferId(BufferTypes::Uvs, vaoId);
        auto normalBufferId = GetBufferId(BufferTypes::Normals, vaoId);
        glBindVertexArray(vaoId);
        UpdateIndicesData(indexBufferId, indices);
        UpdatePositions3DData(positionBufferId, positions, p->locationPosition);
        UpdateColorsData(colorBufferId, colors, p->locationColor);
        UpdateUvsData(uvBufferId, uvs, p->locationUv);
        UpdateNormalsData(normalBufferId, normals, p->locationNormal);
        glBindVertexArray(0);
        if (!p->isBatchDrawing)
            glUseProgram(0);
    }

    void RendererTiles::StartBatchDrawing()
    /*///////////////////////////////////*/
    {
        p->isBatchDrawing = true;
        glUseProgram(GetShaderProgram()->GetProgramId());
        glUniformMatrix4fv(
            p->locationProjection, 1, GL_FALSE,
            glm::value_ptr(CameraGl::Get().GetPerspectiveMatrix()));
        glUniformMatrix4fv(
            p->locationView, 1, GL_FALSE, glm::value_ptr(CameraGl::Get().GetViewMatrix()));
        glm::mat4 model(1.0);
        glUniformMatrix4fv(p->locationModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(p->locationAlpha, 1.0f);
        glm::vec3 viewPos(
            Player::Get().GetSpaceCoord().x, Player::Get().GetSpaceCoord().y,
            Player::Get().GetSpaceCoord().z);
        glUniform3fv(p->locationViewPos, 1, glm::value_ptr(viewPos));
        glm::vec3 fogColorGl(
            GraphicsGl::Get().GetFogColorGround().r, GraphicsGl::Get().GetFogColorGround().g,
            GraphicsGl::Get().GetFogColorGround().b);
        glUniform3fv(p->locationFogColor, 1, glm::value_ptr(fogColorGl));
        glUseProgram(GetShaderProgram()->GetProgramId());
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    }

    void RendererTiles::StopBatchDrawing()
    /*//////////////////////////////////*/
    {
        p->isBatchDrawing = false;
        glUseProgram(0);
        glDisable(GL_CULL_FACE);
    }

    void RendererTiles::DrawImagePolygon(
        int imageNameHash, RenderId vaoId, int vertexCount, bool depthTestOff) const
    /*////////////////////////////////////////////////////////////////////////////*/
    {
        if (depthTestOff)
            glDisable(GL_DEPTH_TEST);
        else
            glEnable(GL_DEPTH_TEST);
        if (!p->isBatchDrawing)
        /*********************/
        {
            glUseProgram(GetShaderProgram()->GetProgramId());
            glUniformMatrix4fv(
                p->locationProjection, 1, GL_FALSE,
                glm::value_ptr(CameraGl::Get().GetPerspectiveMatrix()));
            glUniformMatrix4fv(
                p->locationView, 1, GL_FALSE, glm::value_ptr(CameraGl::Get().GetViewMatrix()));
            glm::mat4 model(1.0);
            glUniformMatrix4fv(p->locationModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1f(p->locationAlpha, 1.0f);
            glm::vec3 viewPos(
                Player::Get().GetSpaceCoord().x, Player::Get().GetSpaceCoord().y,
                Player::Get().GetSpaceCoord().z);
            glUniform3fv(p->locationViewPos, 1, glm::value_ptr(viewPos));
            glm::vec3 fogColorGl(
                GraphicsGl::Get().GetFogColorGround().r, GraphicsGl::Get().GetFogColorGround().g,
                GraphicsGl::Get().GetFogColorGround().b);
            glUniform3fv(p->locationFogColor, 1, glm::value_ptr(fogColorGl));
            glUseProgram(GetShaderProgram()->GetProgramId());
        }
        auto imageId = ImageBank::Get().GetImage(imageNameHash);
        glBindTexture(GL_TEXTURE_2D, imageId);
        glBindVertexArray(vaoId);
        glDrawElements(GL_TRIANGLE_FAN, vertexCount, GL_UNSIGNED_INT, NULL);
        glBindVertexArray(0);
        if (!p->isBatchDrawing)
            glUseProgram(0);
    }

    void RendererTiles::Cleanup()
    /*/////////////////////////*/
    {
        CleanupBase();
    }
}
//////////////////////////////////////////////////////////////////////