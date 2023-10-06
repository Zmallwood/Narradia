//////////////////////////////////////////////////////////////////////
#include "RendererTiles.hpp"
#include "ShaderProgram.hpp"
#include "CameraGl.hpp"
#include "World/Player.hpp"
#include "Engine/Core/Graphics/GraphicsGl.hpp"
#include "Engine/Assets.hpp"
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
            R"(
            #version 330 core
            layout (location = 0) in vec3 in_Position;
            layout (location = 1) in vec4 in_Color;
            layout (location = 2) in vec2 in_TexCoord;
            layout (location = 3) in vec3  in_Normal;
            out vec4 ex_Color;
            out vec2 ex_TexCoord;
            out vec3  ex_Normal;
            out vec3 FragPos;
            uniform mat4 projection;
            uniform mat4 view;
            uniform mat4 model;
            void main() {
               gl_Position = projection * view * vec4(in_Position, 1.0);
               ex_Color = in_Color;
               ex_TexCoord = in_TexCoord;
               ex_Normal = in_Normal;
               FragPos = vec3(model * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0));
            }
            )";
        const GLchar *fragmentShaderSource =
            R"(
            #version 330 core
            precision mediump float;
            vec3 light_direction = vec3(4.5, -3.0, -4.5);
            vec3 light_ambient = vec3(0.8, 0.8, 0.8);
            vec3 light_color = vec3(1.2, 0.7, 0.9);
            in vec4 ex_Color;
            in vec2 ex_TexCoord;
            in vec3 ex_Normal;
            in vec3 FragPos;
            uniform sampler2D TexUnit;
            uniform float mAlpha;
            uniform vec3 viewPos;
            uniform vec3 fogColor;
            out vec4 fragColor;
            float nearPlane = 0.1;
            float farPlane = 65.0;
            float getFogFactor(float d, float nearPlane, float farPlane)
            {
               float FogMax = 1.0f * farPlane;
               float FogMin = 0.5f * farPlane;
               if (d>=FogMax) return 1.0f;
               if (d<=FogMin) return 0.0f;
               return 1.0f - (FogMax - d) / (FogMax - FogMin);
            }
            float getFogFactorAlpha(float d, float nearPlane, float farPlane)
            {
               float FogMax = 1.0f * farPlane;
               float FogMin = 0.7f * farPlane;
               if (d>=FogMax) return 1.0f;
               if (d<=FogMin) return 0.0f;
               return 1.0f - (FogMax - d) / (FogMax - FogMin);
            }
            void main() {
               vec4 color = vec4(ex_Color.rgb, texture(TexUnit, ex_TexCoord).a);
               if (color.a == 0.0)
                  discard;
               vec3 mAmbient = light_ambient * texture(TexUnit, ex_TexCoord).rgb;
               vec3 norm = normalize(ex_Normal);
               vec3 lightDir = normalize(-light_direction);
               float diff = 3.5*max(dot(norm, lightDir), 0.15);
               vec3 mDiffuse = light_color * diff * texture(TexUnit, ex_TexCoord).rgb;
               vec3 result = mAmbient + mDiffuse;
               vec4 result2 = vec4(result.r*color.r, result.g*color.g, result.b*color.b, color.a);
               fragColor = result2;
               float d = distance(viewPos, FragPos);
               float fogFactor = getFogFactor(d, nearPlane, farPlane);
               float alpha = getFogFactorAlpha(d, nearPlane, farPlane);
               fragColor = mix(fragColor, vec4(fogColor, 1.0f), fogFactor);
               fragColor.a = 1-alpha;
            }
            )";
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