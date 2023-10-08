#include "RendererBase.hpp"
#include "ShaderProgram.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class RendererBase::Pimpl {
      public:
        constexpr static int kNumVerticesInRectangle = 4;
        constexpr static int kNumFloatsPerIndex = 1;
        constexpr static int kNumFloatsPerPosition2D = 2;
        constexpr static int kNumFloatsPerPosition3D = 3;
        constexpr static int kNumFloatsPerColor = 4;
        constexpr static int kNumFloatsPerUv = 2;
        constexpr static int kNumFloatsPerNormal = 3;
        std::vector<GLuint> vao_ids_;
        std::map<BufferTypes, std::map<GLuint, GLuint>> vbo_ids_;
        std::shared_ptr<ShaderProgram> shader_program_;
    };

    RendererBase::RendererBase()
        : p(std::make_shared<Pimpl>()) {
        p->shader_program_ = std::make_shared<ShaderProgram>();
    }

    GLuint RendererBase::GenerateNewVertexArrayId() {
        GLuint new_vao_id;
        glGenVertexArrays(1, &new_vao_id);
        p->vao_ids_.push_back(new_vao_id);
        return new_vao_id;
    }

    GLuint RendererBase::GenerateNewBufferId(BufferTypes bufferType, GLuint vaoId) {
        GLuint new_buffer_id;
        glGenBuffers(1, &new_buffer_id);
        p->vbo_ids_[bufferType][vaoId] = new_buffer_id;
        return new_buffer_id;
    }

    GLuint RendererBase::GetBufferId(BufferTypes bufferType, GLuint vaoId) const {
        return p->vbo_ids_.at(bufferType).at(vaoId);
    }

    void
    RendererBase::SetIndicesData(GLuint indicesVboId, int numVertices, const void *data) const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVboId);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, numVertices * Pimpl::kNumFloatsPerIndex * sizeof(float), data,
            GL_DYNAMIC_DRAW);
    }

    void RendererBase::SetPositions2DData(
        GLuint pos2dVboId, int numVertices, const void *data, int layoutLocation) const {
        glBindBuffer(GL_ARRAY_BUFFER, pos2dVboId);
        glBufferData(
            GL_ARRAY_BUFFER, numVertices * Pimpl::kNumFloatsPerPosition2D * sizeof(float), data,
            GL_DYNAMIC_DRAW);
        if (layoutLocation >= 0) {
            glVertexAttribPointer(
                layoutLocation, Pimpl::kNumFloatsPerPosition2D, GL_FLOAT, GL_FALSE, 0,
                (const GLvoid *)0);
            glEnableVertexAttribArray(layoutLocation);
        }
    }

    void RendererBase::SetPositions3DData(
        GLuint pos3dVboId, int numVertices, const void *data, int layoutLocation) const {
        glBindBuffer(GL_ARRAY_BUFFER, pos3dVboId);
        glBufferData(
            GL_ARRAY_BUFFER, sizeof(float) * numVertices * Pimpl::kNumFloatsPerPosition3D, data,
            GL_STATIC_DRAW);
        if (layoutLocation >= 0) {
            glVertexAttribPointer(
                layoutLocation, Pimpl::kNumFloatsPerPosition3D, GL_FLOAT, GL_FALSE, 0,
                (const GLvoid *)0);
            glEnableVertexAttribArray(layoutLocation);
        }
    }

    void RendererBase::SetUvsData(
        GLuint uvVboId, int numVertices, const void *data, int layoutLocation) const {
        glBindBuffer(GL_ARRAY_BUFFER, uvVboId);
        glBufferData(
            GL_ARRAY_BUFFER, numVertices * Pimpl::kNumFloatsPerUv * sizeof(float), data,
            GL_DYNAMIC_DRAW);
        if (layoutLocation >= 0) {
            glVertexAttribPointer(
                layoutLocation, Pimpl::kNumFloatsPerUv, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
            glEnableVertexAttribArray(layoutLocation);
        }
    }

    void RendererBase::SetColorsData(
        GLuint colorVboId, int numVertices, const void *data, int layoutLocation) const {
        glBindBuffer(GL_ARRAY_BUFFER, colorVboId);
        glBufferData(
            GL_ARRAY_BUFFER, numVertices * Pimpl::kNumFloatsPerColor * sizeof(float), data,
            GL_DYNAMIC_DRAW);
        if (layoutLocation >= 0) {
            glVertexAttribPointer(
                layoutLocation, Pimpl::kNumFloatsPerColor, GL_FLOAT, GL_FALSE, 0,
                (const GLvoid *)0);
            glEnableVertexAttribArray(layoutLocation);
        }
    }

    void RendererBase::SetNormalsData(
        GLuint normalVboId, int numVertices, const void *data, int layoutLocation) const {
        glBindBuffer(GL_ARRAY_BUFFER, normalVboId);
        glBufferData(
            GL_ARRAY_BUFFER, sizeof(float) * numVertices * Pimpl::kNumFloatsPerNormal, data,
            GL_STATIC_DRAW);
        if (layoutLocation >= 0) {
            glVertexAttribPointer(
                layoutLocation, Pimpl::kNumFloatsPerNormal, GL_FLOAT, GL_FALSE, 0,
                (const GLvoid *)0);
            glEnableVertexAttribArray(layoutLocation);
        }
    }

    void RendererBase::UpdateIndicesData(GLuint indicesVboId, std::vector<int> &indices) const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVboId);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(float) * indices.size(), indices.data());
    }

    void RendererBase::UpdatePositions2DData(
        GLuint positionsVboId, std::vector<float> &positions, int layoutLocation) const {
        glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * positions.size(), positions.data());
        glVertexAttribPointer(
            layoutLocation, Pimpl::kNumFloatsPerPosition2D, GL_FLOAT, GL_FALSE, 0,
            (const GLvoid *)0);
        glEnableVertexAttribArray(layoutLocation);
    }

    void RendererBase::UpdatePositions3DData(
        GLuint positionsVboId, std::vector<float> &positions, int layoutLocation) const {
        glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * positions.size(), positions.data());
        glVertexAttribPointer(
            layoutLocation, Pimpl::kNumFloatsPerPosition3D, GL_FLOAT, GL_FALSE, 0,
            (const GLvoid *)0);
        glEnableVertexAttribArray(layoutLocation);
    }

    void RendererBase::UpdateUvsData(
        GLuint uvsVboId, std::vector<float> &uvs, int layoutLocation) const {
        glBindBuffer(GL_ARRAY_BUFFER, uvsVboId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * uvs.size(), uvs.data());
        glVertexAttribPointer(
            layoutLocation, Pimpl::kNumFloatsPerUv, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
        glEnableVertexAttribArray(layoutLocation);
    }

    void RendererBase::UpdateColorsData(
        GLuint colorsVboId, std::vector<float> &colors, int layoutLocation) const {
        glBindBuffer(GL_ARRAY_BUFFER, colorsVboId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * colors.size(), colors.data());
        glVertexAttribPointer(
            layoutLocation, Pimpl::kNumFloatsPerColor, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
        glEnableVertexAttribArray(layoutLocation);
    }

    void RendererBase::UpdateNormalsData(
        GLuint normalsVboId, std::vector<float> &normals, int layoutLocation) const {
        glBindBuffer(GL_ARRAY_BUFFER, normalsVboId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * normals.size(), normals.data());
        glVertexAttribPointer(
            layoutLocation, Pimpl::kNumFloatsPerNormal, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
        glEnableVertexAttribArray(layoutLocation);
    }

    void RendererBase::CleanupBase() const {
        for (auto &buffer_type : p->vbo_ids_) {
            for (auto &buffer_entry : buffer_type.second) {
                glDeleteBuffers(1, &buffer_entry.second);
            }
        }
        for (auto vao_id : p->vao_ids_) {
            glDeleteVertexArrays(1, &vao_id);
        }
        if (p->shader_program_)
            p->shader_program_->Cleanup();
    }

    void RendererBase::UseVaoBegin(int vaoId) const {
        glUseProgram(p->shader_program_->GetProgramId());
        glBindVertexArray(vaoId);
    }

    void RendererBase::UseVaoEnd() const {
        glBindVertexArray(0);
        glUseProgram(0);
    }

    GLuint RendererBase::GetUniformLocation(std::string_view varName) {
        return glGetUniformLocation(p->shader_program_->GetProgramId(), varName.data());
    }

    ShaderProgram *RendererBase::GetShaderProgram() const {
        return p->shader_program_.get();
    }

    const int RendererBase::GetNumVerticlesInRectangle() {
        return Pimpl::kNumVerticesInRectangle;
    }
}