//////////////////////////////////////////////////////////////////////
#include "Renderer2DImages.hpp"
#include "ShaderProgram.hpp"
#include "Engine/Assets/ImageBank.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Renderer2DImages::Pimpl
    /*/////////////////////////*/
    {
      public:
        const int locationPosition = 0;
        const int locationColor = 1;
        const int locationUv = 2;
    };

    Renderer2DImages::Renderer2DImages()
        : p(std::make_shared<Pimpl>())
    /*////////////////////////////////*/
    {
        const GLchar *vertexShaderSource =
#include "Shaders/2DImagesVertex.glsl"
            ;
        const GLchar *fragmentShaderSource =
#include "Shaders/2DImagesFragment.glsl"
            ;
        GetShaderProgram()->Create(vertexShaderSource, fragmentShaderSource);
    }

    RenderId Renderer2DImages::NewImage()
    /*/////////////////////////////////*/
    {
        auto vaoId = GenerateNewVertexArrayId();
        UseVaoBegin(vaoId);
        auto indexBufferId = GenerateNewBufferId(BufferTypes::Indices, vaoId);
        auto positionBufferId = GenerateNewBufferId(BufferTypes::Positions, vaoId);
        auto colorBufferId = GenerateNewBufferId(BufferTypes::Colors, vaoId);
        auto uvBufferId = GenerateNewBufferId(BufferTypes::Uvs, vaoId);
        SetIndicesData(indexBufferId, RendererBase::GetNumVerticlesInRectangle(), nullptr);
        SetPositions2DData(positionBufferId, RendererBase::GetNumVerticlesInRectangle(), nullptr);
        SetColorsData(colorBufferId, RendererBase::GetNumVerticlesInRectangle(), nullptr);
        SetUvsData(uvBufferId, RendererBase::GetNumVerticlesInRectangle(), nullptr);
        UseVaoEnd();
        return vaoId;
    }

    void Renderer2DImages::DrawImage(
        int imageNameHash, RenderId vaoId, const RectangleF &rectangle, Color color) const
    /*//////////////////////////////////////////////////////////////////////////////////*/
    {
        auto glRect = rectangle.ToGlRect();
        Vertex2F vertices[RendererBase::kNumVerticesInRectangle];
        vertices[0].position = {glRect.x, glRect.y - glRect.height};
        vertices[1].position = {glRect.x, glRect.y};
        vertices[2].position = {glRect.x + glRect.width, glRect.y};
        vertices[3].position = {glRect.x + glRect.width, glRect.y - glRect.height};
        vertices[0].color = color;
        vertices[1].color = color;
        vertices[2].color = color;
        vertices[3].color = color;
        vertices[0].uv = {0.0f, 1.0f};
        vertices[1].uv = {0.0f, 0.0f};
        vertices[2].uv = {1.0f, 0.0f};
        vertices[3].uv = {1.0f, 1.0f};
        glDisable(GL_DEPTH_TEST);
        auto imageId = ImageBank::Get().GetImage(imageNameHash);
        glBindTexture(GL_TEXTURE_2D, imageId);
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
        auto indexBufferId = GetBufferId(BufferTypes::Indices, vaoId);
        auto positionBufferId = GetBufferId(BufferTypes::Positions, vaoId);
        auto colorBufferId = GetBufferId(BufferTypes::Colors, vaoId);
        auto uvBufferId = GetBufferId(BufferTypes::Uvs, vaoId);
        UpdateIndicesData(indexBufferId, indices);
        UpdatePositions2DData(positionBufferId, positions, p->locationPosition);
        UpdateColorsData(colorBufferId, colors, p->locationColor);
        UpdateUvsData(uvBufferId, uvs, p->locationUv);
        glDrawElements(
            GL_TRIANGLE_FAN, RendererBase::GetNumVerticlesInRectangle(), GL_UNSIGNED_INT, NULL);
        UseVaoEnd();
    }

    void Renderer2DImages::DrawImage(
        const std::string_view &imageName, RenderId vaoId, const RectangleF &rectangle,
        Color color) const
    /*///////////////////////////////////////////////////////////////////////////////*/
    {
        DrawImage(Hash(imageName), vaoId, rectangle, color);
    }

    void Renderer2DImages::Cleanup()
    /*////////////////////////////*/
    {
        CleanupBase();
    }
}
//////////////////////////////////////////////////////////////////////