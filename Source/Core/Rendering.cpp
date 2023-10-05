//////////////////////////////////////////////////////////////////////
#include "Rendering.hpp"
#include "Assets.hpp"
#include "World/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void SdlDeleter::operator()(SDL_Window *window) const
    /*/////////////////////////////////////////////////*/ {
        SDL_DestroyWindow(window);
    }

    void SdlDeleter::operator()(SDL_Renderer *renderer) const
    /*/////////////////////////////////////////////////////*/ {
        SDL_DestroyRenderer(renderer);
    }

    void SdlDeleter::operator()(SDL_Surface *surface) const
    /*///////////////////////////////////////////////////*/ {
        SDL_FreeSurface(surface);
    }

    void SdlDeleter::operator()(SDL_Texture *texture) const
    /*///////////////////////////////////////////////////*/ {
        SDL_DestroyTexture(texture);
    }

    void SdlDeleter::operator()(TTF_Font *font) const
    /*/////////////////////////////////////////////*/ {
        TTF_CloseFont(font);
    }

    class ShaderProgram::Pimpl
    /*//////////////////////*/ {
      public:
        GLuint InitVertexShader(const GLchar *);
        GLuint InitFragShader(const GLchar *);

        GLuint vertexShader = 0;
        GLuint fragShader = 0;
        GLuint programId;
    };

    ShaderProgram::ShaderProgram()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {
    }

    bool ShaderProgram::Create(const GLchar *vertexShaderSrc, const GLchar *fragShaderSrc)
    /*//////////////////////////////////////////////////////////////////////////////////*/ {
        auto success = true;
        p->programId = glCreateProgram();
        auto vertexShaderCompiled = p->InitVertexShader(vertexShaderSrc);
        if (vertexShaderCompiled != GL_TRUE)
        /**********************************/ {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR, "Shader error", "Unable to compile vertex shader.", nullptr);
            printf("Unable to compile vertex shader %d!\n", p->vertexShader);
            success = false;
        } else
        /****/ {
            glAttachShader(p->programId, p->vertexShader);
            auto fragShaderCompiled = p->InitFragShader(fragShaderSrc);
            if (fragShaderCompiled != GL_TRUE)
            /********************************/ {
                SDL_ShowSimpleMessageBox(
                    SDL_MESSAGEBOX_ERROR, "Shader error", "Unable to compile fragment shader.",
                    nullptr);
                printf("Unable to compile fragment shader %d!\n", p->fragShader);
                success = false;
            } else
            /****/ {
                glAttachShader(p->programId, p->fragShader);
                glLinkProgram(p->programId);
                GLint program_success = GL_TRUE;
                glGetProgramiv(p->programId, GL_LINK_STATUS, &program_success);
                if (program_success != GL_TRUE)
                /*****************************/ {
                    SDL_ShowSimpleMessageBox(
                        SDL_MESSAGEBOX_ERROR, "Shader error", "Error linking shader program.",
                        nullptr);
                    printf("Error linking program %d!\n", p->programId);
                    success = false;
                }
            }
        }
        glDeleteShader(p->vertexShader);
        glDeleteShader(p->fragShader);
        return success;
    }

    void ShaderProgram::Cleanup() const
    /*///////////////////////////////*/ {
        glDeleteProgram(p->programId);
    }

    GLuint ShaderProgram::GetProgramId()
    /*////////////////////////////////*/ {
        return p->programId;
    }

    GLuint ShaderProgram::Pimpl::InitVertexShader(const GLchar *vertexShaderSource)
    /*///////////////////////////////////////////////////////////////////////////*/ {
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        GLint vertexShaderCompiled = GL_FALSE;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderCompiled);
        return vertexShaderCompiled;
    }

    GLuint ShaderProgram::Pimpl::InitFragShader(const GLchar *fragmentShaderSource)
    /*///////////////////////////////////////////////////////////////////////////*/ {
        fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragShader);
        GLint fragShaderCompiled = GL_FALSE;
        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragShaderCompiled);
        return fragShaderCompiled;
    }

    class RndrBase::Pimpl
    /*/////////////////*/ {
      public:
        constexpr static int kNumVerticesInRectangle = 4;
        constexpr static int kNumFloatsPerIndex = 1;
        constexpr static int kNumFloatsPerPosition2D = 2;
        constexpr static int kNumFloatsPerPosition3D = 3;
        constexpr static int kNumFloatsPerColor = 4;
        constexpr static int kNumFloatsPerUv = 2;
        constexpr static int kNumFloatsPerNormal = 3;
        std::vector<GLuint> vaoIds;
        std::map<BufferTypes, std::map<GLuint, GLuint>> vboIds;
        std::shared_ptr<ShaderProgram> shaderProgram;
    };

    RndrBase::RndrBase()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {
        p->shaderProgram = std::make_shared<ShaderProgram>();
    }

    GLuint RndrBase::GenerateNewVertexArrayId()
    /*///////////////////////////////////////*/ {
        GLuint newVaoId;
        glGenVertexArrays(1, &newVaoId);
        p->vaoIds.push_back(newVaoId);
        return newVaoId;
    }

    GLuint RndrBase::GenerateNewBufferId(BufferTypes bufferType, GLuint vaoId)
    /*//////////////////////////////////////////////////////////////////////*/ {
        GLuint newBufferId;
        glGenBuffers(1, &newBufferId);
        p->vboIds[bufferType][vaoId] = newBufferId;
        return newBufferId;
    }

    GLuint RndrBase::GetBufferId(BufferTypes bufferType, GLuint vaoId) const
    /*////////////////////////////////////////////////////////////////////*/ {
        return p->vboIds.at(bufferType).at(vaoId);
    }

    void RndrBase::SetIndicesData(GLuint indicesVboId, int numVertices, const void *data) const
    /*///////////////////////////////////////////////////////////////////////////////////////*/ {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVboId);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, numVertices * Pimpl::kNumFloatsPerIndex * sizeof(float), data,
            GL_DYNAMIC_DRAW);
    }

    void RndrBase::SetPositions2DData(
        GLuint pos2dVboId, int numVertices, const void *data, int layoutLocation) const
    /*///////////////////////////////////////////////////////////////////////////////*/
    {
        glBindBuffer(GL_ARRAY_BUFFER, pos2dVboId);
        glBufferData(
            GL_ARRAY_BUFFER, numVertices * Pimpl::kNumFloatsPerPosition2D * sizeof(float), data,
            GL_DYNAMIC_DRAW);
        if (layoutLocation >= 0)
        /**********************/ {
            glVertexAttribPointer(
                layoutLocation, Pimpl::kNumFloatsPerPosition2D, GL_FLOAT, GL_FALSE, 0,
                (const GLvoid *)0);
            glEnableVertexAttribArray(layoutLocation);
        }
    }

    void RndrBase::SetPositions3DData(
        GLuint pos3dVboId, int numVertices, const void *data, int layoutLocation) const
    /*///////////////////////////////////////////////////////////////////////////////*/
    {
        glBindBuffer(GL_ARRAY_BUFFER, pos3dVboId);
        glBufferData(
            GL_ARRAY_BUFFER, sizeof(float) * numVertices * Pimpl::kNumFloatsPerPosition3D, data,
            GL_STATIC_DRAW);
        if (layoutLocation >= 0)
        /**********************/ {
            glVertexAttribPointer(
                layoutLocation, Pimpl::kNumFloatsPerPosition3D, GL_FLOAT, GL_FALSE, 0,
                (const GLvoid *)0);
            glEnableVertexAttribArray(layoutLocation);
        }
    }

    void RndrBase::SetUvsData(
        GLuint uvVboId, int numVertices, const void *data, int layoutLocation) const
    /*/////////////////////////////////////////////////////////////////////////////////////////////*/
    {
        glBindBuffer(GL_ARRAY_BUFFER, uvVboId);
        glBufferData(
            GL_ARRAY_BUFFER, numVertices * Pimpl::kNumFloatsPerUv * sizeof(float), data,
            GL_DYNAMIC_DRAW);
        if (layoutLocation >= 0)
        /**********************/ {
            glVertexAttribPointer(
                layoutLocation, Pimpl::kNumFloatsPerUv, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
            glEnableVertexAttribArray(layoutLocation);
        }
    }

    void RndrBase::SetColorsData(
        GLuint colorVboId, int numVertices, const void *data, int layoutLocation) const
    /*///////////////////////////////////////////////////////////////////////////////*/
    {
        glBindBuffer(GL_ARRAY_BUFFER, colorVboId);
        glBufferData(
            GL_ARRAY_BUFFER, numVertices * Pimpl::kNumFloatsPerColor * sizeof(float), data,
            GL_DYNAMIC_DRAW);
        if (layoutLocation >= 0)
        /**********************/ {
            glVertexAttribPointer(
                layoutLocation, Pimpl::kNumFloatsPerColor, GL_FLOAT, GL_FALSE, 0,
                (const GLvoid *)0);
            glEnableVertexAttribArray(layoutLocation);
        }
    }

    void RndrBase::SetNormalsData(
        GLuint normalVboId, int numVertices, const void *data, int layoutLocation) const
    /*////////////////////////////////////////////////////////////////////////////////*/
    {
        glBindBuffer(GL_ARRAY_BUFFER, normalVboId);
        glBufferData(
            GL_ARRAY_BUFFER, sizeof(float) * numVertices * Pimpl::kNumFloatsPerNormal, data,
            GL_STATIC_DRAW);
        if (layoutLocation >= 0)
        /**********************/ {
            glVertexAttribPointer(
                layoutLocation, Pimpl::kNumFloatsPerNormal, GL_FLOAT, GL_FALSE, 0,
                (const GLvoid *)0);
            glEnableVertexAttribArray(layoutLocation);
        }
    }

    void RndrBase::UpdateIndicesData(GLuint indicesVboId, std::vector<int> &indices) const
    /*//////////////////////////////////////////////////////////////////////////////////*/ {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVboId);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(float) * indices.size(), indices.data());
    }

    void RndrBase::UpdatePositions2DData(
        GLuint positionsVboId, std::vector<float> &positions, int layoutLocation) const
    /*///////////////////////////////////////////////////////////////////////////////*/ {
        glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * positions.size(), positions.data());
        glVertexAttribPointer(
            layoutLocation, Pimpl::kNumFloatsPerPosition2D, GL_FLOAT, GL_FALSE, 0,
            (const GLvoid *)0);
        glEnableVertexAttribArray(layoutLocation);
    }

    void RndrBase::UpdatePositions3DData(
        GLuint positionsVboId, std::vector<float> &positions, int layoutLocation) const
    /*///////////////////////////////////////////////////////////////////////////////*/ {
        glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * positions.size(), positions.data());
        glVertexAttribPointer(
            layoutLocation, Pimpl::kNumFloatsPerPosition3D, GL_FLOAT, GL_FALSE, 0,
            (const GLvoid *)0);
        glEnableVertexAttribArray(layoutLocation);
    }

    void RndrBase::UpdateUvsData(GLuint uvsVboId, std::vector<float> &uvs, int layoutLocation) const
    /*////////////////////////////////////////////////////////////////////////////////////////*/ {
        glBindBuffer(GL_ARRAY_BUFFER, uvsVboId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * uvs.size(), uvs.data());
        glVertexAttribPointer(
            layoutLocation, Pimpl::kNumFloatsPerUv, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
        glEnableVertexAttribArray(layoutLocation);
    }

    void RndrBase::UpdateColorsData(
        GLuint colorsVboId, std::vector<float> &colors, int layoutLocation) const
    /*/////////////////////////////////////////////////////////////////////////*/ {
        glBindBuffer(GL_ARRAY_BUFFER, colorsVboId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * colors.size(), colors.data());
        glVertexAttribPointer(
            layoutLocation, Pimpl::kNumFloatsPerColor, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
        glEnableVertexAttribArray(layoutLocation);
    }

    void RndrBase::UpdateNormalsData(
        GLuint normalsVboId, std::vector<float> &normals, int layoutLocation) const
    /*///////////////////////////////////////////////////////////////////////////*/ {
        glBindBuffer(GL_ARRAY_BUFFER, normalsVboId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * normals.size(), normals.data());
        glVertexAttribPointer(
            layoutLocation, Pimpl::kNumFloatsPerNormal, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
        glEnableVertexAttribArray(layoutLocation);
    }

    void RndrBase::CleanupBase() const
    /*//////////////////////////////*/ {
        for (auto &bufferType : p->vboIds)
        /********************************/ {
            for (auto &bufferEntry : bufferType.second)
            /*****************************************/ {
                glDeleteBuffers(1, &bufferEntry.second);
            }
        }
        for (auto vaoId : p->vaoIds)
        /**************************/ {
            glDeleteVertexArrays(1, &vaoId);
        }
        if (p->shaderProgram) p->shaderProgram->Cleanup();
    }

    void RndrBase::UseVaoBegin(int vaoId) const
    /*///////////////////////////////////////*/ {
        glUseProgram(p->shaderProgram->GetProgramId());
        glBindVertexArray(vaoId);
    }

    void RndrBase::UseVaoEnd() const
    /*////////////////////////////*/ {
        glBindVertexArray(0);
        glUseProgram(0);
    }

    GLuint RndrBase::GetUniformLocation(std::string_view varName)
    /*/////////////////////////////////////////////////////////*/ {
        return glGetUniformLocation(p->shaderProgram->GetProgramId(), varName.data());
    }

    ShaderProgram *RndrBase::GetShaderProgram() const
    /*/////////////////////////////////////////////*/ {
        return p->shaderProgram.get();
    }

    const int RndrBase::GetNumVerticlesInRectangle()
    /*////////////////////////////////////////////*/ {
        return Pimpl::kNumVerticesInRectangle;
    }

    class CameraGl::Pimpl
    /*/////////////////*/ {
      public:
        glm::mat4 perspectiveMatrix;
        glm::mat4 viewMatrix;
    };

    CameraGl::CameraGl()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {
    }

    const glm::mat4 &CameraGl::GetPerspectiveMatrix()
    /*/////////////////////////////////////////////*/ {
        return p->perspectiveMatrix;
    }

    const glm::mat4 &CameraGl::GetViewMatrix()
    /*//////////////////////////////////////*/ {
        return p->viewMatrix;
    }

    void CameraGl::SetPerspectiveMatrix(glm::mat4 newPerspectiveMatrix)
    /*///////////////////////////////////////////////////////////////*/ {
        p->perspectiveMatrix = newPerspectiveMatrix;
    }

    void CameraGl::SetViewMatrix(glm::mat4 newViewMatrix)
    /*/////////////////////////////////////////////////*/ {
        p->viewMatrix = newViewMatrix;
    }

    class Renderer2DImages::Pimpl
    /*/////////////////////////*/ {
      public:
        const int locationPosition = 0;
        const int locationColor = 1;
        const int locationUv = 2;
    };

    Renderer2DImages::Renderer2DImages()
        : p(std::make_shared<Pimpl>())
    /*////////////////////////////////*/ {
        const GLchar *vertexShaderSource =
            R"(
            #version 330 core
            layout (location = 0) in vec2 in_Position;
            layout (location = 1) in vec4 in_Color;
            layout (location = 2) in vec2 in_TexCoord;
            out vec4 ex_Color;
            out vec2 ex_TexCoord;
            void main() {
               gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0);
               ex_Color = in_Color;
               ex_TexCoord = in_TexCoord;
            }
            )";
        const GLchar *fragmentShaderSource =
            R"(
            #version 330 core
            precision mediump float;
            in vec4 ex_Color;
            in vec2 ex_TexCoord;
            uniform sampler2D TexUnit;
            out vec4 fragColor;
            void main() {
               vec4 texColor = texture(TexUnit, ex_TexCoord);
               fragColor = ex_Color*0.0 + texColor;
            }
            )";
        GetShaderProgram()->Create(vertexShaderSource, fragmentShaderSource);
    }

    RenderId Renderer2DImages::NewImage()
    /*/////////////////////////////////*/ {
        auto vaoId = GenerateNewVertexArrayId();
        UseVaoBegin(vaoId);
        auto indexBufferId = GenerateNewBufferId(BufferTypes::Indices, vaoId);
        auto positionBufferId = GenerateNewBufferId(BufferTypes::Positions, vaoId);
        auto colorBufferId = GenerateNewBufferId(BufferTypes::Colors, vaoId);
        auto uvBufferId = GenerateNewBufferId(BufferTypes::Uvs, vaoId);
        SetIndicesData(indexBufferId, RndrBase::GetNumVerticlesInRectangle(), nullptr);
        SetPositions2DData(positionBufferId, RndrBase::GetNumVerticlesInRectangle(), nullptr);
        SetColorsData(colorBufferId, RndrBase::GetNumVerticlesInRectangle(), nullptr);
        SetUvsData(uvBufferId, RndrBase::GetNumVerticlesInRectangle(), nullptr);
        UseVaoEnd();
        return vaoId;
    }

    void Renderer2DImages::DrawImage(
        int imageNameHash, RenderId vaoId, const RectangleF &rectangle, Color color) const
    /*//////////////////////////////////////////////////////////////////////////////////*/
    {
        auto glRect = rectangle.ToGlRect();
        Vertex2F vertices[RndrBase::kNumVerticesInRectangle];
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
        std::vector<int> indices(RndrBase::GetNumVerticlesInRectangle());
        std::iota(std::begin(indices), std::end(indices), 0);
        std::vector<float> positions;
        std::vector<float> colors;
        std::vector<float> uvs;
        for (auto &vertex : vertices)
        /***************************/ {
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
            GL_TRIANGLE_FAN, RndrBase::GetNumVerticlesInRectangle(), GL_UNSIGNED_INT, NULL);
        UseVaoEnd();
    }

    void Renderer2DImages::DrawImage(
        const std::string_view &imageName, RenderId vaoId, const RectangleF &rectangle,
        Color color) const
    /*///////////////////////////////////////////////////////////////////////////////*/ {
        DrawImage(Hash(imageName), vaoId, rectangle, color);
    }

    void Renderer2DImages::Cleanup()
    /*////////////////////////////*/ {
        CleanupBase();
    }

    class Renderer2DSolidColors::Pimpl
    /*//////////////////////////////*/ {
      public:
        const int locationPosition = 0;
        const int locationColor = 1;
    };

    Renderer2DSolidColors::Renderer2DSolidColors()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////////////////*/ {
        const GLchar *vertexShaderSource =
            R"(
            #version 330 core
            layout (location = 0) in vec2 in_Position;
            layout (location = 1) in vec4 in_Color;
            out vec4 ex_Color;
            void main() {
               gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0);
               ex_Color = in_Color;
            }
            )";
        const GLchar *fragmentShaderSource =
            R"(
            #version 330 core
            precision mediump float;
            in vec4 ex_Color;
            out vec4 fragColor;
            void main() {
               fragColor = ex_Color;
            }
            )";
        GetShaderProgram()->Create(vertexShaderSource, fragmentShaderSource);
    }

    RenderId Renderer2DSolidColors::NewRectangle()
    /*//////////////////////////////////////////*/ {
        auto vaoId = GenerateNewVertexArrayId();
        UseVaoBegin(vaoId);
        auto indexBufferId = GenerateNewBufferId(BufferTypes::Indices, vaoId);
        auto positionBufferId = GenerateNewBufferId(BufferTypes::Positions, vaoId);
        auto colorBufferId = GenerateNewBufferId(BufferTypes::Colors, vaoId);
        SetIndicesData(indexBufferId, RndrBase::GetNumVerticlesInRectangle(), nullptr);
        SetPositions2DData(positionBufferId, RndrBase::GetNumVerticlesInRectangle(), nullptr);
        SetColorsData(colorBufferId, RndrBase::GetNumVerticlesInRectangle(), nullptr);
        UseVaoEnd();
        return vaoId;
    }

    void
    Renderer2DSolidColors::FillRectangle(RenderId vaoId, const RectangleF &rect, Color Color) const
    /*///////////////////////////////////////////////////////////////////////////////////////////*/
    {
        auto glRect = rect.ToGlRect();
        Vertex2F vertices[RndrBase::kNumVerticesInRectangle];
        vertices[0].position = {glRect.x, glRect.y - glRect.height};
        vertices[1].position = {glRect.x, glRect.y};
        vertices[2].position = {glRect.x + glRect.width, glRect.y};
        vertices[3].position = {glRect.x + glRect.width, glRect.y - glRect.height};
        vertices[0].color = Color;
        vertices[1].color = Color;
        vertices[2].color = Color;
        vertices[3].color = Color;
        glDisable(GL_DEPTH_TEST);
        std::vector<int> indices(RndrBase::GetNumVerticlesInRectangle());
        std::iota(std::begin(indices), std::end(indices), 0);
        std::vector<float> positions;
        std::vector<float> colors;
        for (auto &vertex : vertices)
        /***************************/ {
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
            GL_TRIANGLE_FAN, RndrBase::GetNumVerticlesInRectangle(), GL_UNSIGNED_INT, NULL);
        UseVaoEnd();
    }

    void Renderer2DSolidColors::Cleanup()
    /*/////////////////////////////////*/ {
        CleanupBase();
    }

    class RendererBillboardImages::Pimpl
    /*////////////////////////////////*/ {
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
    /*//////////////////////////////////////////////*/ {
        const GLchar *vertexShaderSource =
            R"(
            #version 330 core
            layout (location = 0) in vec2 in_Position;
            layout (location = 1) in vec2 in_TexCoord;
            out vec2 ex_TexCoord;
            uniform mat4 projection;
            uniform mat4 view;
            uniform vec3 particleCenterWorldspace;
            uniform vec2 billboardSize;
            void main() {
               gl_Position = projection * view * vec4(particleCenterWorldspace, 1.0);
               gl_Position /= gl_Position.w;
               gl_Position.xy += in_Position * billboardSize;
               ex_TexCoord = in_TexCoord;
            }
            )";
        const GLchar *fragmentShaderSource =
            R"(
            #version 330 core
            precision mediump float;
            in vec2 ex_TexCoord;
            uniform sampler2D TexUnit;
            out vec4 fragColor;
            void main() {
               if (texture(TexUnit, ex_TexCoord).a == 0.0)
                  discard;
               fragColor = texture(TexUnit, ex_TexCoord).rgba;
            }
            )";
        GetShaderProgram()->Create(vertexShaderSource, fragmentShaderSource);
        p->locationProjection = GetUniformLocation("projection");
        p->locationView = GetUniformLocation("view");
        p->locationParticleCenterWorldspace = GetUniformLocation("particleCenterWorldspace");
        p->locationBillboardSize = GetUniformLocation("billboardSize");
    }

    RenderId RendererBillboardImages::NewBillboardImage()
    /*/////////////////////////////////////////////////*/ {
        auto vaoId = GenerateNewVertexArrayId();
        UseVaoBegin(vaoId);
        auto indexBufferId = GenerateNewBufferId(BufferTypes::Indices, vaoId);
        auto positionBufferId = GenerateNewBufferId(BufferTypes::Positions, vaoId);
        auto uvBufferId = GenerateNewBufferId(BufferTypes::Uvs, vaoId);
        const auto numVertices = RndrBase::GetNumVerticlesInRectangle();
        SetIndicesData(indexBufferId, numVertices, nullptr);
        SetPositions2DData(positionBufferId, numVertices, nullptr);
        SetUvsData(uvBufferId, numVertices, nullptr);
        UseVaoEnd();
        return vaoId;
    }

    void RendererBillboardImages::DrawBillboardImage(
        int imageNameHash, RenderId vaoId, Point3F position, RectangleF &bounds,
        SizeF billboardSize) const
    /*////////////////////////////////////////////////////////////////////////*/
    {
        Vertex3F vertices[RndrBase::kNumVerticesInRectangle];
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
        std::vector<int> indices(RndrBase::GetNumVerticlesInRectangle());
        std::iota(std::begin(indices), std::end(indices), 0);
        std::vector<float> positions;
        std::vector<float> uvs;
        for (auto &v : vertices)
        /**********************/ {
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
            GL_TRIANGLE_FAN, RndrBase::GetNumVerticlesInRectangle(), GL_UNSIGNED_INT, NULL);
        UseVaoEnd();
    }

    void RendererBillboardImages::Cleanup()
    /*///////////////////////////////////*/ {
        CleanupBase();
    }

    class RendererModels::Pimpl
    /*///////////////////////*/ {
      public:
        struct BodyData
        /*///////////*/ {
            RenderId bodyId = 0;
            int numVertices = 0;
            int imageNameHash = 0;
        };

        const int locationPosition = 0;
        const int locationColor = 1;
        const int locationUv = 2;
        const int locationNormal = 3;
        int locationProjection = -1;
        int locationView = -1;
        int locationAlpha = -1;
        int locationModel = -1;
        int locationModelNoTranslation = -1;
        int locationColorMod = -1;
        int locationViewPos = -1;
        int locationFogColor = -1;
        int locationNoFog = -1;
        int locationNoLighting = -1;
        std::map<int, std::map<int, std::map<float, const BodyData>>> modelIds;
        std::map<int, std::map<float, RenderId>> timelines;
        float globalAnimSpeed = 1.0f;
        bool isBatchDrawing = false;
    };

    RendererModels::RendererModels()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {
        const GLchar *vertexShaderSource =
            R"(
            #version 330 core
            layout (location = 0) in vec3 in_Position;
            layout (location = 1) in vec4 in_Color;
            layout (location = 2) in vec2 in_TexCoord;
            layout (location = 3) in vec3  in_Normal;
            out vec4 ex_Color;
            out vec2 ex_TexCoord;
            out vec3 ex_Normal;
            out vec3 FragPos;
            uniform mat4 projection;
            uniform mat4 view;
            uniform mat4 model;
            uniform mat4 modelNoTranslation;
            void main() {
               vec4 newVertex;
               vec4 newNormal;
               newVertex.xyz = in_Position;
               newVertex.w = 1.0;
               newNormal.xyz = in_Normal;
               newNormal.w = 1.0;
               ex_Color = in_Color;
               gl_Position = projection * view * model * newVertex;
               ex_TexCoord = in_TexCoord;
               ex_Normal = (modelNoTranslation * newNormal).xyz;
               FragPos = vec3(model * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0));
            }
            )";
        const GLchar *fragmentShaderSource =
            R"(
            #version 330 core
            precision mediump float;
            vec3 light_direction = -vec3(0.0, -0.5, 4.5);
            vec3 light_ambient = vec3(0.65, 0.65, 0.65);
            vec3 light_color = vec3(0.6, 0.6, 0.5);
            in vec4 ex_Color;
            in vec2 ex_TexCoord;
            in vec3 ex_Normal;
            in float FogCoord;
            in vec3 FragPos;
            uniform sampler2D TexUnit;
            uniform float mAlpha;
            uniform vec3 mColorMod;
            uniform vec3 viewPos;
            uniform vec3 fogColor;
            uniform float noFog;
            uniform float noLighting;
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
               vec3 result;
               vec3 mAmbient = light_ambient * texture(TexUnit, ex_TexCoord).rgb;
               if (noLighting == 0.0)
               {
                  vec3 norm = normalize(ex_Normal);
                  vec3 lightDir =normalize(-light_direction);
                  float diff =  1.5*max(dot(norm, lightDir), 0.15);
                  vec3 mDiffuse = light_color * diff * texture(TexUnit, ex_TexCoord).rgb;
                  result = mAmbient + mDiffuse;
               }
               else
               {
                  result = mAmbient;
               }
               fragColor = vec4(result*mAlpha, texture(TexUnit, ex_TexCoord).a);
               fragColor.r = fragColor.r*mColorMod.r;
               fragColor.g = fragColor.g*mColorMod.g;
               fragColor.b = fragColor.b*mColorMod.b;
               if (noFog == 0.0)
               {
                  float d = distance(viewPos, FragPos);
                  float fogFactor = getFogFactor(d, nearPlane, farPlane);
                  float alpha = getFogFactorAlpha(d, nearPlane, farPlane);
                  fragColor = mix(fragColor, vec4(fogColor, 1.0f), fogFactor);
                  if (fragColor.a == 1.0)
                        fragColor.a = 1-alpha;
               }
            }
            )";
        GetShaderProgram()->Create(vertexShaderSource, fragmentShaderSource);
        p->locationView = GetUniformLocation("view");
        p->locationProjection = GetUniformLocation("projection");
        p->locationAlpha = GetUniformLocation("mAlpha");
        p->locationModel = GetUniformLocation("model");
        p->locationModelNoTranslation = GetUniformLocation("modelNoTranslation");
        p->locationColorMod = GetUniformLocation("mColorMod");
        p->locationViewPos = GetUniformLocation("viewPos");
        p->locationFogColor = GetUniformLocation("fogColor");
        p->locationNoFog = GetUniformLocation("noFog");
        p->locationNoLighting = GetUniformLocation("noLighting");
    }

    void RendererModels::NewModel(int modelNameHash)
    /*////////////////////////////////////////////*/ {
        auto model = ModelBank::Get().GetModel(modelNameHash);
        p->modelIds.insert(
            {modelNameHash, std::map<int, std::map<float, const Pimpl::BodyData>>()});
        auto iBody = 0;
        for (auto &body : model->modelParts)
        /**********************************/ {
            auto &timelines = body->timeline->keyframes;
            for (auto &keyframe : timelines)
            /******************************/ {
                auto keyframeTime = keyframe.first;
                auto animKeyBodyKeyframe = keyframe.second;
                auto vertexCount = animKeyBodyKeyframe->vertices.size();
                auto bodyKeyframeId = NewBodyKeyframe(modelNameHash, keyframeTime, vertexCount);
                if (p->modelIds.at(modelNameHash).count(iBody) == 0)
                    p->modelIds.at(modelNameHash)
                        .insert({iBody, std::map<float, const Pimpl::BodyData>()});
                Pimpl::BodyData bodyData;
                bodyData.bodyId = bodyKeyframeId;
                bodyData.imageNameHash = body->texNameHash;
                bodyData.numVertices = vertexCount;
                p->modelIds.at(modelNameHash).at(iBody).insert({keyframeTime, bodyData});
                auto &modelKeyframe = body->timeline->keyframes.at(keyframeTime);
                std::vector<Vertex3F> vertices;
                std::vector<Point3F> normals;
                auto &animVertices = modelKeyframe->vertices;
                for (auto v : animVertices)
                /*************************/ {
                    Vertex3F v3f;
                    v3f.position = v.position;
                    v3f.color = v.color;
                    v3f.uv = v.uv;
                    vertices.push_back(v3f);
                    Point3F n3f;
                    n3f.x = v.normal.x;
                    n3f.y = v.normal.y;
                    n3f.z = v.normal.z;
                    normals.push_back(n3f);
                }
                NewBodyKeyframeGeometry(
                    body->texNameHash, keyframeTime, bodyKeyframeId, vertices, normals);
            }
            iBody++;
        }
    }

    RenderId RendererModels::NewBodyKeyframe(int modelNameHash, float msTime, int numVertices)
    /*//////////////////////////////////////////////////////////////////////////////////////*/ {
        auto vaoId = GenerateNewVertexArrayId();
        if (p->timelines.count(modelNameHash) == 0)
            p->timelines.insert({modelNameHash, std::map<float, RenderId>()});
        p->timelines.at(modelNameHash).insert({msTime, vaoId});
        return vaoId;
    }

    void RendererModels::NewBodyKeyframeGeometry(
        int imageNamehash, float msTime, RenderId vaoId, std::vector<Vertex3F> vertices,
        std::vector<Point3F> vertexNormals)
    /*////////////////////////////////////////////////////////////////////////////////*/ {
        glEnable(GL_DEPTH_TEST);
        UseVaoBegin(vaoId);
        glUniformMatrix4fv(
            p->locationProjection, 1, GL_FALSE, value_ptr(CameraGl::Get().GetPerspectiveMatrix()));
        glUniformMatrix4fv(
            p->locationView, 1, GL_FALSE, value_ptr(CameraGl::Get().GetViewMatrix()));
        glUniform1f(p->locationAlpha, 1.0f);
        auto imageId = ImageBank::Get().GetImage(imageNamehash);
        glBindTexture(GL_TEXTURE_2D, imageId);
        std::vector<int> indices(vertices.size());
        std::iota(std::begin(indices), std::end(indices), 0);
        std::vector<float> positions;
        std::vector<float> colors;
        std::vector<float> uvs;
        std::vector<float> normals;
        auto i = 0;
        for (auto &vertex : vertices)
        /***************************/ {
            positions.push_back(vertex.position.x);
            positions.push_back(vertex.position.y);
            positions.push_back(vertex.position.z);
            colors.push_back(vertex.color.r);
            colors.push_back(vertex.color.g);
            colors.push_back(vertex.color.b);
            colors.push_back(vertex.color.a);
            uvs.push_back(vertex.uv.x);
            uvs.push_back(vertex.uv.y);
            if (vertexNormals.size() > i)
            /***************************/ {
                auto vertexNormal = vertexNormals.at(i);
                normals.push_back(vertexNormal.x);
                normals.push_back(-vertexNormal.y);
                normals.push_back(vertexNormal.z);
            }
            i++;
        }
        auto indexBufferId = GenerateNewBufferId(BufferTypes::Indices, vaoId);
        auto positionBufferId = GenerateNewBufferId(BufferTypes::Positions, vaoId);
        auto colorBufferid = GenerateNewBufferId(BufferTypes::Colors, vaoId);
        auto uvBufferId = GenerateNewBufferId(BufferTypes::Uvs, vaoId);
        auto normalBufferId = GenerateNewBufferId(BufferTypes::Normals, vaoId);
        auto numVertices = vertices.size();
        SetIndicesData(indexBufferId, numVertices, indices.data());
        SetPositions3DData(positionBufferId, numVertices, positions.data(), p->locationPosition);
        SetColorsData(colorBufferid, numVertices, colors.data(), p->locationColor);
        SetUvsData(uvBufferId, numVertices, uvs.data(), p->locationUv);
        SetNormalsData(normalBufferId, numVertices, normals.data(), p->locationNormal);
        UseVaoEnd();
    }

    void RendererModels::StartBatchDrawing()
    /*////////////////////////////////////*/ {
        p->isBatchDrawing = true;
        glEnable(GL_DEPTH_TEST);
        glUseProgram(GetShaderProgram()->GetProgramId());
        glUniformMatrix4fv(
            p->locationProjection, 1, GL_FALSE, value_ptr(CameraGl::Get().GetPerspectiveMatrix()));
        glUniformMatrix4fv(
            p->locationView, 1, GL_FALSE, glm::value_ptr(CameraGl::Get().GetViewMatrix()));
    }

    void RendererModels::StopBatchDrawing()
    /*///////////////////////////////////*/ {
        p->isBatchDrawing = false;
    }

    void RendererModels::DrawModel(
        int modelNameHash, float msTime, Point3F position, float rotation, float scaling,
        float brightness, glm::vec3 colorMod, bool noFog, bool noLighting) const
    /*/////////////////////////////////////////////////////////////////////////////////*/
    {
        if (p->modelIds.count(modelNameHash) == 0) return;
        if (!p->isBatchDrawing)
        /*********************/ {
            glEnable(GL_DEPTH_TEST);
            glUseProgram(GetShaderProgram()->GetProgramId());
            glUniformMatrix4fv(
                p->locationProjection, 1, GL_FALSE,
                value_ptr(CameraGl::Get().GetPerspectiveMatrix()));
            auto &viewMatrix = CameraGl::Get().GetViewMatrix();
            glUniformMatrix4fv(p->locationView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        }
        auto modelMatrix = glm::rotate(
            glm::scale(
                glm::translate(glm::mat4(1.0), glm::vec3(position.x, position.y, position.z)),
                glm::vec3(scaling, scaling, scaling)),
            glm::radians(rotation), glm::vec3(0, 1, 0));
        auto modelNoTranslationMatrix = glm::rotate(
            glm::scale(
                glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0)),
                glm::vec3(scaling, scaling, scaling)),
            glm::radians(rotation), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(p->locationModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(
            p->locationModelNoTranslation, 1, GL_FALSE, glm::value_ptr(modelNoTranslationMatrix));
        glUniform3fv(p->locationColorMod, 1, glm::value_ptr(colorMod));
        glm::vec3 viewPos(
            Player::Get().GetSpaceCoord().x, Player::Get().GetSpaceCoord().y,
            Player::Get().GetSpaceCoord().z);
        glUniform3fv(p->locationViewPos, 1, glm::value_ptr(viewPos));
        glm::vec3 fogColorGl(
            GraphicsGl::Get().GetFogColorObjects().r, GraphicsGl::Get().GetFogColorObjects().g,
            GraphicsGl::Get().GetFogColorObjects().b);
        glUniform3fv(p->locationFogColor, 1, glm::value_ptr(fogColorGl));
        glUniform1f(p->locationAlpha, brightness);
        glUniform1f(p->locationNoFog, noFog ? 1.0f : 0.0f);
        glUniform1f(p->locationNoLighting, noLighting ? 1.0f : 0.0f);
        auto &allNodes = p->modelIds.at(modelNameHash);
        auto pModel = ModelBank::Get().GetModel(modelNameHash);
        int msTimeUsed;
        if (pModel->animDuration == 0)
            msTimeUsed = 0;
        else
            msTimeUsed = static_cast<int>(msTime * p->globalAnimSpeed) % pModel->animDuration;
        auto &modelData = allNodes;
        for (auto &entry : allNodes)
        /**************************/ {
            auto &timeline = entry.second;
            auto foundTime = -1.0f;
            const Pimpl::BodyData *pBodyData = nullptr;
            for (auto &keyframe : timeline)
            /*****************************/ {
                auto time = keyframe.first;
                if (msTimeUsed >= time) foundTime = time;
            }
            auto &bodyData = timeline.at(foundTime);
            glBindVertexArray(bodyData.bodyId);
            auto imageId = ImageBank::Get().GetImage(bodyData.imageNameHash);
            glBindTexture(GL_TEXTURE_2D, imageId);
            glDrawElements(GL_TRIANGLES, bodyData.numVertices, GL_UNSIGNED_INT, NULL);
        }
        glBindVertexArray(0);
        if (!p->isBatchDrawing) glUseProgram(0);
    }

    void RendererModels::DrawModelsMany(
        int modelNameHash, float msTime, std::vector<Point3F> positions,
        std::vector<float> rotations, std::vector<float> scalings, std::vector<float> brightnesses,
        std::vector<glm::vec3> colorMods) const
    /*///////////////////////////////////////////////////////////////////////////////////////////*/
    {
        if (p->modelIds.count(modelNameHash) == 0) return;
        if (!p->isBatchDrawing)
        /*********************/ {
            glEnable(GL_DEPTH_TEST);
            glUseProgram(GetShaderProgram()->GetProgramId());
            glUniformMatrix4fv(
                p->locationProjection, 1, GL_FALSE,
                value_ptr(CameraGl::Get().GetPerspectiveMatrix()));
            auto &viewMatrix = CameraGl::Get().GetViewMatrix();
            glUniformMatrix4fv(p->locationView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        }
        auto &allNodes = p->modelIds.at(modelNameHash);
        auto pModel = ModelBank::Get().GetModel(modelNameHash);
        int msTimeUsed;
        if (pModel->animDuration == 0)
            msTimeUsed = 0;
        else
            msTimeUsed = static_cast<int>(msTime * p->globalAnimSpeed) % pModel->animDuration;
        auto &modelData = allNodes;
        for (auto &entry : allNodes)
        /**************************/ {
            auto &timeline = entry.second;
            auto foundTime = -1.0f;
            const Pimpl::BodyData *pBodyData = nullptr;
            for (auto &keyframe : timeline)
            /*****************************/ {
                auto time = keyframe.first;
                if (msTimeUsed >= time) foundTime = time;
            }
            auto &bodyData = timeline.at(foundTime);
            auto imageId = ImageBank::Get().GetImage(bodyData.imageNameHash);
            for (auto i = 0; i < positions.size(); i++)
            /*****************************************/ {
                auto position = positions.at(i);
                auto rotation = rotations.at(i);
                auto scaling = scalings.at(i);
                auto colorMod = colorMods.at(i);
                auto brightness = brightnesses.at(i);
                auto modelMatrix = glm::rotate(
                    glm::scale(
                        glm::translate(
                            glm::mat4(1.0), glm::vec3(position.x, position.y, position.z)),
                        glm::vec3(scaling, scaling, scaling)),
                    glm::radians(rotation), glm::vec3(0, 1, 0));
                auto modelNoTranslationMatrix = glm::rotate(
                    glm::scale(
                        glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0)),
                        glm::vec3(scaling, scaling, scaling)),
                    glm::radians(rotation), glm::vec3(0, 1, 0));
                glUniformMatrix4fv(p->locationModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
                glUniformMatrix4fv(
                    p->locationModelNoTranslation, 1, GL_FALSE,
                    glm::value_ptr(modelNoTranslationMatrix));
                glUniform3fv(p->locationColorMod, 1, glm::value_ptr(colorMod));
                glm::vec3 viewPos(
                    Player::Get().GetSpaceCoord().x, Player::Get().GetSpaceCoord().y,
                    Player::Get().GetSpaceCoord().z);
                glUniform3fv(p->locationViewPos, 1, glm::value_ptr(viewPos));
                glm::vec3 fogColorGl(
                    GraphicsGl::Get().GetFogColorObjects().r,
                    GraphicsGl::Get().GetFogColorObjects().g,
                    GraphicsGl::Get().GetFogColorObjects().b);
                glUniform3fv(p->locationFogColor, 1, glm::value_ptr(fogColorGl));
                glUniform1f(p->locationAlpha, brightness);
                glUniform1f(p->locationNoFog, 0.0f);
                glUniform1f(p->locationNoLighting, 0.0f);
                glBindVertexArray(bodyData.bodyId);
                glBindTexture(GL_TEXTURE_2D, imageId);
                glDrawElements(GL_TRIANGLES, bodyData.numVertices, GL_UNSIGNED_INT, NULL);
            }
        }
        glBindVertexArray(0);
        if (!p->isBatchDrawing) glUseProgram(0);
    }

    void RendererModels::Cleanup()
    /*//////////////////////////*/ {
        CleanupBase();
    }

    class RendererTiles::Pimpl
    /*//////////////////////*/ {
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
    /*//////////////////////////////*/ {
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
    /*////////////////////////////////////////////////////*/ {
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
    /*/////////////////////////////*/ {
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
    /*////////////////////////////////////////////////////////////////////////////////////*/ {
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
        if (!p->isBatchDrawing) glUseProgram(GetShaderProgram()->GetProgramId());
        std::vector<int> indices(vertices.size());
        std::iota(std::begin(indices), std::end(indices), 0);
        std::vector<float> positions;
        std::vector<float> colors;
        std::vector<float> uvs;
        std::vector<float> normals;
        for (auto &vertex : vertices)
        /***************************/ {
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
        if (!p->isBatchDrawing) glUseProgram(0);
    }

    void RendererTiles::StartBatchDrawing()
    /*///////////////////////////////////*/ {
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
    /*//////////////////////////////////*/ {
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
        /*********************/ {
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
        if (!p->isBatchDrawing) glUseProgram(0);
    }

    void RendererTiles::Cleanup()
    /*/////////////////////////*/ {
        CleanupBase();
    }

    class Font::Pimpl
    /*/////////////*/ {
      public:
        std::shared_ptr<TTF_Font> sdlFont = nullptr;
        std::shared_ptr<TTF_Font> sdlFontOutline = nullptr;
        static constexpr int kFontOutlineWidth = 2;
    };

    Font::Font(std::string_view fontFileName, int fontSize)
        : p(std::make_shared<Pimpl>())
    /*///////////////////////////////////////////////////*/ {
        p->sdlFont =
            std::shared_ptr<TTF_Font>(TTF_OpenFont(fontFileName.data(), fontSize), SdlDeleter());
        p->sdlFontOutline =
            std::shared_ptr<TTF_Font>(TTF_OpenFont(fontFileName.data(), fontSize), SdlDeleter());
        TTF_SetFontOutline(p->sdlFontOutline.get(), Pimpl::kFontOutlineWidth);
    }

    TTF_Font *Font::GetSdlFont() const
    /*//////////////////////////////*/ {
        return p->sdlFont.get();
    }

    TTF_Font *Font::GetSdlFontOutline() const
    /*/////////////////////////////////////*/ {
        return p->sdlFontOutline.get();
    }

    constexpr int Font::GetFontOutlineWidth()
    /*/////////////////////////////////////*/ {
        return Pimpl::kFontOutlineWidth;
    }

    class TextRenderer::Pimpl
    /*/////////////////////*/ {
      public:
        void RenderText(
            RenderId, std::string_view, Color, bool, TextSizes, std::string &, SizeF &) const;
        const std::string CreateBlankTexGetName();

        const std::string relFontsPath = "Resources/Fonts/";
        const Color outlineColor = Colors::black;
        std::map<TextSizes, std::shared_ptr<const Font>> fonts;
        std::map<RenderId, std::string> uniqueNameIds;
        int idCounter = 0;
        std::map<RenderId, MultiLineText> multiLines;
    };

    TextRenderer::TextRenderer()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {
        TTF_Init();
        auto fontPath =
            std::string(SDL_GetBasePath()) + p->relFontsPath + "PartyConfettiRegular-eZOn3.ttf";
        p->fonts.insert({TextSizes::_20, std::make_shared<Font>(fontPath.c_str(), 20)});
        p->fonts.insert({TextSizes::_26, std::make_shared<Font>(fontPath.c_str(), 26)});
    }

    RenderId TextRenderer::NewString()
    /*//////////////////////////////*/ {
        auto uniqueName = p->CreateBlankTexGetName();
        auto rendIdImageRect = Renderer2DImages::Get().NewImage();
        p->uniqueNameIds.insert({rendIdImageRect, uniqueName});
        return rendIdImageRect;
    }

    RenderId TextRenderer::NewBillboardString()
    /*///////////////////////////////////////*/ {
        auto uniqueName = p->CreateBlankTexGetName();
        auto rendIdBboardTexRect = RendererBillboardImages::Get().NewBillboardImage();
        p->uniqueNameIds.insert({rendIdBboardTexRect, uniqueName});
        return rendIdBboardTexRect;
    }

    RenderId TextRenderer::NewMultiLineString(int numLines, float width)
    /*////////////////////////////////////////////////////////////////*/ {
        MultiLineText multiLineText;
        multiLineText.width = width;
        for (auto i = 0; i < numLines; i++)
            multiLineText.renderIds.push_back(NewString());
        p->multiLines.insert({multiLineText.renderIds.at(0), multiLineText});
        return multiLineText.renderIds.at(0);
    }

    void TextRenderer::Pimpl::RenderText(
        RenderId vaoId, std::string_view text, Color color, bool centerAlign, TextSizes textSize,
        std::string &outUniqueNameId, SizeF &outSize) const
    /*/////////////////////////////////////////////////////////////////////////////////////////*/ {
        auto font = fonts.at(textSize)->GetSdlFont();
        if (!font) return;
        auto sdlColor = color.ToSdlColor();
        auto outlineSdlColor = outlineColor.ToSdlColor();
        auto textOutlineSurface = TTF_RenderText_Blended(
            fonts.at(textSize)->GetSdlFontOutline(), text.data(), outlineSdlColor);
        if (!textOutlineSurface) return;
        auto textSurface = TTF_RenderText_Blended(font, text.data(), sdlColor);
        glEnable(GL_TEXTURE_2D);
        auto uniqueNameId = uniqueNameIds.at(vaoId);
        auto imageId = ImageBank::Get().GetImage(uniqueNameId.c_str());
        glBindTexture(GL_TEXTURE_2D, imageId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        auto w = textOutlineSurface->w;
        auto h = textOutlineSurface->h;
        auto image = SDL_CreateRGBSurface(
            SDL_SWSURFACE, w, h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        auto area = SDL_Rect{
            Font::GetFontOutlineWidth(), Font::GetFontOutlineWidth(),
            textSurface ? textSurface->w : 0, textSurface ? textSurface->h : 0};
        auto areaOutline = SDL_Rect{
            0, 0, textOutlineSurface ? textOutlineSurface->w : 0,
            textOutlineSurface ? textOutlineSurface->h : 0};
        SDL_BlitSurface(textOutlineSurface, &area, image, &areaOutline);
        SDL_BlitSurface(textSurface, &areaOutline, image, &areaOutline);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
            image->pixels);
        auto canvasSize = GetCanvasSize();
        auto width = textSurface ? static_cast<float>(textSurface->w) / canvasSize.width : 0;
        auto height = textSurface ? static_cast<float>(textSurface->h) / canvasSize.height : 0;
        outUniqueNameId = uniqueNameId;
        outSize = {width, height};
        SDL_FreeSurface(image);
        SDL_FreeSurface(textSurface);
        SDL_FreeSurface(textOutlineSurface);
    }

    void TextRenderer::DrawMultiLineString(
        RenderId glId, const std::string &text, Point2F position, Color color, bool centerAlign,
        TextSizes textSize) const
    /*////////////////////////////////////////////////////////////////////////////////////////*/
    {
        int fullTextW;
        int fullTextH;
        auto multiLineObject = p->multiLines.at(glId);
        auto canvasSize = GetCanvasSize();
        auto aspectRatio = GetAspectRatio();
        TTF_SizeText(p->fonts.at(textSize)->GetSdlFont(), text.c_str(), &fullTextW, &fullTextH);
        auto totalTextWidthF = static_cast<float>(fullTextW) / canvasSize.width;
        auto numLines = totalTextWidthF / multiLineObject.width;
        auto numCharactersPerLine = text.length() / numLines;
        auto lineHeight = static_cast<float>(fullTextH) / canvasSize.height;
        for (auto i = 0; i < numLines; i++)
        /*********************************/ {
            auto lineText = text.substr(i * numCharactersPerLine, numCharactersPerLine + 1);
            std::string uniqueNameId;
            SizeF size;
            p->RenderText(
                multiLineObject.renderIds.at(i), lineText, color, centerAlign, textSize,
                uniqueNameId, size);
            auto rect =
                RectangleF{position.x, position.y + i * lineHeight, size.width, size.height};
            int textW;
            int textH;
            TTF_SizeText(p->fonts.at(textSize)->GetSdlFont(), text.c_str(), &textW, &textH);
            rect.y -= static_cast<float>(textH / GetAspectRatio()) / canvasSize.height / 2.0f;
            if (centerAlign)
                rect.x -= static_cast<float>(textW) / static_cast<float>(canvasSize.height) / 2.0f /
                          GetAspectRatio();
            Renderer2DImages::Get().DrawImage(uniqueNameId, multiLineObject.renderIds.at(i), rect);
        }
    }

    void TextRenderer::DrawString(
        RenderId vaoId, std::string_view text, Point2F position, Color color, bool centerAlign,
        TextSizes textSize) const
    /*///////////////////////////////////////////////////////////////////////////////////////*/
    {
        std::string uniqueNameId;
        SizeF size;
        p->RenderText(vaoId, text, color, centerAlign, textSize, uniqueNameId, size);
        auto canvasSize = GetCanvasSize();
        auto rect = RectangleF{position.x, position.y, size.width, size.height};
        int textW;
        int textH;
        TTF_SizeText(p->fonts.at(textSize)->GetSdlFont(), text.data(), &textW, &textH);
        rect.y -= static_cast<float>(textH / GetAspectRatio()) / canvasSize.height / 2.0f;
        if (centerAlign)
            rect.x -= static_cast<float>(textW) / static_cast<float>(canvasSize.height) / 2.0f /
                      GetAspectRatio();
        Renderer2DImages::Get().DrawImage(uniqueNameId, vaoId, rect);
    }

    void TextRenderer::DrawBillboardString(
        RenderId vaoId, std::string_view text, Point3F position, SizeF billboardSize, Color color,
        bool centerAlign, TextSizes textSize) const
    /*//////////////////////////////////////////////////////////////////////////////////////////*/
    {
        std::string uniqueNameId;
        SizeF size;
        p->RenderText(vaoId, text, color, centerAlign, textSize, uniqueNameId, size);
        auto canvasSize = GetCanvasSize();
        int text_w;
        int text_h;
        TTF_SizeText(p->fonts.at(textSize)->GetSdlFont(), text.data(), &text_w, &text_h);
        size.height = 1.0f;
        size.width = size.height / static_cast<float>(text_h) * text_w / billboardSize.width *
                     billboardSize.height / GetAspectRatio();
        auto rect = RectangleF{-size.width / 2, -size.height / 2, size.width, size.height};
        rect.y -= static_cast<float>(text_h / GetAspectRatio()) / canvasSize.height / 2.0f;
        if (centerAlign)
            rect.x -= static_cast<float>(text_w) / static_cast<float>(canvasSize.height) / 2.0f /
                      GetAspectRatio();
        RendererBillboardImages::Get().DrawBillboardImage(
            Hash(uniqueNameId), vaoId, position, rect, billboardSize);
    }

    const std::string TextRenderer::Pimpl::CreateBlankTexGetName()
    /*//////////////////////////////////////////////////////////*/ {
        auto id = idCounter++;
        auto uniqueName = "RenderedImage" + std::to_string(id);
        ImageBank::Get().GetBlankTextImage(uniqueName);
        return uniqueName;
    }
}