//////////////////////////////////////////////////////////////////////
#include "RendererBillboardImages.hpp"
#include "CameraGl.hpp"
#include "Engine/Assets/ImageBank.hpp"
#include "ShaderProgram.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class RendererBillboardImages::Pimpl
    /*////////////////////////////////*/
    {
      public:
        const int locationPosition = 0;
        const int locationUv = 1;
        int locationProjection = -1;
        int locationView = -1;
        int locationParticleCenterWorldspace = -1;
        int locationBillboardSize = -1;
    };

    RendererBillboardImages::RendererBillboardImages()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////////////////////*/
    {
        const GLchar *vertexShaderSource =
#include "Shaders/BillboardImagesVertex.glsl"
            ;
        const GLchar *fragmentShaderSource =
#include "Shaders/BillboardImagesFragment.glsl"
            ;
        GetShaderProgram()->Create(vertexShaderSource, fragmentShaderSource);
        p->locationProjection = GetUniformLocation("projection");
        p->locationView = GetUniformLocation("view");
        p->locationParticleCenterWorldspace = GetUniformLocation("particleCenterWorldspace");
        p->locationBillboardSize = GetUniformLocation("billboardSize");
    }

    RenderId
    RendererBillboardImages::NewBillboardImage()
    /*/////////////////////////////////////////////////*/
    {
        auto vaoId = GenerateNewVertexArrayId();
        UseVaoBegin(vaoId);
        auto indexBufferId = GenerateNewBufferId(BufferTypes::Indices, vaoId);
        auto positionBufferId = GenerateNewBufferId(BufferTypes::Positions, vaoId);
        auto uvBufferId = GenerateNewBufferId(BufferTypes::Uvs, vaoId);
        const auto numVertices = RendererBase::GetNumVerticlesInRectangle();
        SetIndicesData(indexBufferId, numVertices, nullptr);
        SetPositions2DData(positionBufferId, numVertices, nullptr);
        SetUvsData(uvBufferId, numVertices, nullptr);
        UseVaoEnd();
        return vaoId;
    }

    void
    RendererBillboardImages::DrawBillboardImage(
        int imageNameHash, RenderId vaoId, Point3F position, RectangleF &bounds,
        SizeF billboardSize) const
    /*////////////////////////////////////////////////////////////////////////*/
    {
        Vertex3F vertices[RendererBase::kNumVerticesInRectangle];
        vertices[0].uv = {0, 0};
        vertices[1].uv = {1, 0};
        vertices[2].uv = {1, 1};
        vertices[3].uv = {0, 1};
        auto glRect = bounds.InvertVertically();
        vertices[0].position = {glRect.x, glRect.y};
        vertices[1].position = {glRect.x + glRect.width, glRect.y};
        vertices[2].position = {glRect.x + glRect.width, glRect.y + glRect.height};
        vertices[3].position = {glRect.x, glRect.y + glRect.height};
        glEnable(GL_DEPTH_TEST);
        UseVaoBegin(vaoId);
        glUniformMatrix4fv(
            p->locationProjection, 1, GL_FALSE,
            glm::value_ptr(CameraGl::Get().GetPerspectiveMatrix()));
        glUniformMatrix4fv(
            p->locationView, 1, GL_FALSE, glm::value_ptr(CameraGl::Get().GetViewMatrix()));
        glm::vec3 centerGl = {position.x, position.y, position.z};
        glUniform3fv(p->locationParticleCenterWorldspace, 1, &centerGl[0]);
        glm::vec2 glmBillboardSize = {billboardSize.width, billboardSize.height};
        glUniform2fv(p->locationBillboardSize, 1, &glmBillboardSize[0]);
        auto imageId = ImageBank::Get().GetImage(imageNameHash);
        glBindTexture(GL_TEXTURE_2D, imageId);
        std::vector<int> indices(RendererBase::GetNumVerticlesInRectangle());
        std::iota(std::begin(indices), std::end(indices), 0);
        std::vector<float> positions;
        std::vector<float> uvs;
        for (auto &v : vertices)
        /**********************/
        {
            positions.push_back(v.position.x);
            positions.push_back(v.position.y);
            uvs.push_back(v.uv.x);
            uvs.push_back(v.uv.y);
        }
        auto indexBufferId = GetBufferId(BufferTypes::Indices, vaoId);
        auto positionBufferId = GetBufferId(BufferTypes::Positions, vaoId);
        auto uvBufferId = GetBufferId(BufferTypes::Uvs, vaoId);
        UpdateIndicesData(indexBufferId, indices);
        UpdatePositions2DData(positionBufferId, positions, p->locationPosition);
        UpdateUvsData(uvBufferId, uvs, p->locationUv);
        glDrawElements(
            GL_TRIANGLE_FAN, RendererBase::GetNumVerticlesInRectangle(), GL_UNSIGNED_INT, NULL);
        UseVaoEnd();
    }

    void
    RendererBillboardImages::Cleanup()
    /*///////////////////////////////////*/
    {
        CleanupBase();
    }
}
//////////////////////////////////////////////////////////////////////