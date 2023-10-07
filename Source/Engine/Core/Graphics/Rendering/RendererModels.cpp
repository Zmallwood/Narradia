#include "RendererModels.hpp"
#include "CameraGl.hpp"
#include "Engine/Assets/ImageBank.hpp"
#include "Engine/Assets/ModelBank.hpp"
#include "Engine/Core/Graphics/GraphicsGl.hpp"
#include "Engine/Core/ModelStructure/Model.hpp"
#include "ShaderProgram.hpp"
#include "World/Actors/Player.hpp"
#include "BodyData.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class RendererModels::Pimpl
    /*///////////////////////*/
    {
      public:
        const int kLocationPosition = 0;
        const int kLocationColor = 1;
        const int kLocationUv = 2;
        const int kLocationNormal = 3;
        int location_projection_ = -1;
        int location_view_ = -1;
        int location_alpha_ = -1;
        int location_model_ = -1;
        int location_model_no_translation_ = -1;
        int location_color_mod_ = -1;
        int location_view_pos_ = -1;
        int location_fog_color_ = -1;
        int location_no_fog_ = -1;
        int location_no_lighting_ = -1;
        std::map<int, std::map<int, std::map<float, const BodyData>>> model_ids_;
        std::map<int, std::map<float, RenderId>> timelines_;
        float global_animation_speed_ = 1.0f;
        bool is_batch_drawing_ = false;
    };

    RendererModels::RendererModels()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
        const GLchar *vertex_shader_source =
#include "Shaders/ModelsVertex.glsl"
            ;
        const GLchar *fragment_shader_source =
#include "Shaders/ModelsFragment.glsl"
            ;
        GetShaderProgram()->Create(vertex_shader_source, fragment_shader_source);
        p->location_view_ = GetUniformLocation("view");
        p->location_projection_ = GetUniformLocation("projection");
        p->location_alpha_ = GetUniformLocation("mAlpha");
        p->location_model_ = GetUniformLocation("model");
        p->location_model_no_translation_ = GetUniformLocation("modelNoTranslation");
        p->location_color_mod_ = GetUniformLocation("mColorMod");
        p->location_view_pos_ = GetUniformLocation("viewPos");
        p->location_fog_color_ = GetUniformLocation("fogColor");
        p->location_no_fog_ = GetUniformLocation("noFog");
        p->location_no_lighting_ = GetUniformLocation("noLighting");
    }

    void
    RendererModels::NewModel(int modelNameHash)
    /*///////////////////////////////////////*/
    {
        auto model = ModelBank::Get()->GetModel(modelNameHash);
        p->model_ids_.insert(
            {modelNameHash, std::map<int, std::map<float, const BodyData>>()});
        auto i_body = 0;
        for (auto &body : model->modelParts)
        /**********************************/
        {
            auto &timelines = body->timeline->keyframes;
            for (auto &keyframe : timelines)
            /******************************/
            {
                auto keyframe_time = keyframe.first;
                auto anim_key_body_keyframe = keyframe.second;
                auto vertex_count = anim_key_body_keyframe->vertices.size();
                auto body_keyframe_id = NewBodyKeyframe(modelNameHash, keyframe_time, vertex_count);
                if (p->model_ids_.at(modelNameHash).count(i_body) == 0)
                    p->model_ids_.at(modelNameHash)
                        .insert({i_body, std::map<float, const BodyData>()});
                BodyData body_data;
                body_data.bodyId = body_keyframe_id;
                body_data.imageNameHash = body->texNameHash;
                body_data.numVertices = vertex_count;
                p->model_ids_.at(modelNameHash).at(i_body).insert({keyframe_time, body_data});
                auto &model_keyframe = body->timeline->keyframes.at(keyframe_time);
                std::vector<Vertex3F> vertices;
                std::vector<Point3F> normals;
                auto &anim_vertices = model_keyframe->vertices;
                for (auto v : anim_vertices)
                /*************************/
                {
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
                    body->texNameHash, keyframe_time, body_keyframe_id, vertices, normals);
            }
            i_body++;
        }
    }

    RenderId
    RendererModels::NewBodyKeyframe(int modelNameHash, float msTime, int numVertices)
    /*/////////////////////////////////////////////////////////////////////////////*/
    {
        auto vao_id = GenerateNewVertexArrayId();
        if (p->timelines_.count(modelNameHash) == 0)
            p->timelines_.insert({modelNameHash, std::map<float, RenderId>()});
        p->timelines_.at(modelNameHash).insert({msTime, vao_id});
        return vao_id;
    }

    void
    RendererModels::NewBodyKeyframeGeometry(
        int imageNamehash, float msTime, RenderId vaoId, std::vector<Vertex3F> vertices,
        std::vector<Point3F> vertexNormals)
    /*////////////////////////////////////////////////////////////////////////////////*/
    {
        glEnable(GL_DEPTH_TEST);
        UseVaoBegin(vaoId);
        glUniformMatrix4fv(
            p->location_projection_, 1, GL_FALSE, value_ptr(CameraGl::Get()->GetPerspectiveMatrix()));
        glUniformMatrix4fv(
            p->location_view_, 1, GL_FALSE, value_ptr(CameraGl::Get()->GetViewMatrix()));
        glUniform1f(p->location_alpha_, 1.0f);
        auto image_id = ImageBank::Get()->GetImage(imageNamehash);
        glBindTexture(GL_TEXTURE_2D, image_id);
        std::vector<int> indices(vertices.size());
        std::iota(std::begin(indices), std::end(indices), 0);
        std::vector<float> positions;
        std::vector<float> colors;
        std::vector<float> uvs;
        std::vector<float> normals;
        auto i = 0;
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
            if (vertexNormals.size() > i)
            /***************************/
            {
                auto vertexNormal = vertexNormals.at(i);
                normals.push_back(vertexNormal.x);
                normals.push_back(-vertexNormal.y);
                normals.push_back(vertexNormal.z);
            }
            i++;
        }
        auto index_buffer_id = GenerateNewBufferId(BufferTypes::Indices, vaoId);
        auto position_buffer_id = GenerateNewBufferId(BufferTypes::Positions, vaoId);
        auto color_buffer_id = GenerateNewBufferId(BufferTypes::Colors, vaoId);
        auto uv_buffer_id = GenerateNewBufferId(BufferTypes::Uvs, vaoId);
        auto normal_buffer_id = GenerateNewBufferId(BufferTypes::Normals, vaoId);
        auto num_vertices = vertices.size();
        SetIndicesData(index_buffer_id, num_vertices, indices.data());
        SetPositions3DData(position_buffer_id, num_vertices, positions.data(), p->kLocationPosition);
        SetColorsData(color_buffer_id, num_vertices, colors.data(), p->kLocationColor);
        SetUvsData(uv_buffer_id, num_vertices, uvs.data(), p->kLocationUv);
        SetNormalsData(normal_buffer_id, num_vertices, normals.data(), p->kLocationNormal);
        UseVaoEnd();
    }

    void
    RendererModels::StartBatchDrawing()
    /*///////////////////////////////*/
    {
        p->is_batch_drawing_ = true;
        glEnable(GL_DEPTH_TEST);
        glUseProgram(GetShaderProgram()->GetProgramId());
        glUniformMatrix4fv(
            p->location_projection_, 1, GL_FALSE, value_ptr(CameraGl::Get()->GetPerspectiveMatrix()));
        glUniformMatrix4fv(
            p->location_view_, 1, GL_FALSE, glm::value_ptr(CameraGl::Get()->GetViewMatrix()));
    }

    void
    RendererModels::StopBatchDrawing()
    /*//////////////////////////////*/
    {
        p->is_batch_drawing_ = false;
    }

    void
    RendererModels::DrawModel(
        int modelNameHash, float msTime, Point3F position, float rotation, float scaling,
        float brightness, glm::vec3 colorMod, bool noFog, bool noLighting) const
    /*/////////////////////////////////////////////////////////////////////////////////*/
    {
        if (p->model_ids_.count(modelNameHash) == 0)
            return;
        if (!p->is_batch_drawing_)
        /*********************/
        {
            glEnable(GL_DEPTH_TEST);
            glUseProgram(GetShaderProgram()->GetProgramId());
            glUniformMatrix4fv(
                p->location_projection_, 1, GL_FALSE,
                value_ptr(CameraGl::Get()->GetPerspectiveMatrix()));
            auto &view_matrix = CameraGl::Get()->GetViewMatrix();
            glUniformMatrix4fv(p->location_view_, 1, GL_FALSE, glm::value_ptr(view_matrix));
        }
        auto model_matrix = glm::rotate(
            glm::scale(
                glm::translate(glm::mat4(1.0), glm::vec3(position.x, position.y, position.z)),
                glm::vec3(scaling, scaling, scaling)),
            glm::radians(rotation), glm::vec3(0, 1, 0));
        auto model_no_translation_matrix = glm::rotate(
            glm::scale(
                glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0)),
                glm::vec3(scaling, scaling, scaling)),
            glm::radians(rotation), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(p->location_model_, 1, GL_FALSE, glm::value_ptr(model_matrix));
        glUniformMatrix4fv(
            p->location_model_no_translation_, 1, GL_FALSE, glm::value_ptr(model_no_translation_matrix));
        glUniform3fv(p->location_color_mod_, 1, glm::value_ptr(colorMod));
        glm::vec3 viewPos(
            Player::Get()->GetSpaceCoord().x, Player::Get()->GetSpaceCoord().y,
            Player::Get()->GetSpaceCoord().z);
        glUniform3fv(p->location_view_pos_, 1, glm::value_ptr(viewPos));
        glm::vec3 fogColorGl(
            GraphicsGl::Get()->GetFogColorObjects().r, GraphicsGl::Get()->GetFogColorObjects().g,
            GraphicsGl::Get()->GetFogColorObjects().b);
        glUniform3fv(p->location_fog_color_, 1, glm::value_ptr(fogColorGl));
        glUniform1f(p->location_alpha_, brightness);
        glUniform1f(p->location_no_fog_, noFog ? 1.0f : 0.0f);
        glUniform1f(p->location_no_lighting_, noLighting ? 1.0f : 0.0f);
        auto &all_nodes = p->model_ids_.at(modelNameHash);
        auto p_model = ModelBank::Get()->GetModel(modelNameHash);
        int ms_time_used;
        if (p_model->animDuration == 0)
            ms_time_used = 0;
        else
            ms_time_used = static_cast<int>(msTime * p->global_animation_speed_) % p_model->animDuration;
        auto &model_data = all_nodes;
        for (auto &entry : all_nodes)
        /**************************/
        {
            auto &timeline = entry.second;
            auto found_time = -1.0f;
            const BodyData *p_body_data = nullptr;
            for (auto &keyframe : timeline)
            /*****************************/
            {
                auto time = keyframe.first;
                if (ms_time_used >= time)
                    found_time = time;
            }
            auto &body_data = timeline.at(found_time);
            glBindVertexArray(body_data.bodyId);
            auto image_id = ImageBank::Get()->GetImage(body_data.imageNameHash);
            glBindTexture(GL_TEXTURE_2D, image_id);
            glDrawElements(GL_TRIANGLES, body_data.numVertices, GL_UNSIGNED_INT, NULL);
        }
        glBindVertexArray(0);
        if (!p->is_batch_drawing_)
            glUseProgram(0);
    }

    void
    RendererModels::DrawModelsMany(
        int modelNameHash, float msTime, std::vector<Point3F> positions,
        std::vector<float> rotations, std::vector<float> scalings, std::vector<float> brightnesses,
        std::vector<glm::vec3> colorMods) const
    /*///////////////////////////////////////////////////////////////////////////////////////////*/
    {
        if (p->model_ids_.count(modelNameHash) == 0)
            return;
        if (!p->is_batch_drawing_)
        /*********************/
        {
            glEnable(GL_DEPTH_TEST);
            glUseProgram(GetShaderProgram()->GetProgramId());
            glUniformMatrix4fv(
                p->location_projection_, 1, GL_FALSE,
                value_ptr(CameraGl::Get()->GetPerspectiveMatrix()));
            auto &view_matrix = CameraGl::Get()->GetViewMatrix();
            glUniformMatrix4fv(p->location_view_, 1, GL_FALSE, glm::value_ptr(view_matrix));
        }
        auto &all_nodes = p->model_ids_.at(modelNameHash);
        auto p_model = ModelBank::Get()->GetModel(modelNameHash);
        int ms_time_used;
        if (p_model->animDuration == 0)
            ms_time_used = 0;
        else
            ms_time_used = static_cast<int>(msTime * p->global_animation_speed_) % p_model->animDuration;
        auto &model_data = all_nodes;
        for (auto &entry : all_nodes)
        /***************************/
        {
            auto &timeline = entry.second;
            auto found_time = -1.0f;
            const BodyData *p_body_data = nullptr;
            for (auto &keyframe : timeline)
            /*****************************/
            {
                auto time = keyframe.first;
                if (ms_time_used >= time)
                    found_time = time;
            }
            auto &body_data = timeline.at(found_time);
            auto image_id = ImageBank::Get()->GetImage(body_data.imageNameHash);
            for (auto i = 0; i < positions.size(); i++)
            /*****************************************/
            {
                auto position = positions.at(i);
                auto rotation = rotations.at(i);
                auto scaling = scalings.at(i);
                auto colorMod = colorMods.at(i);
                auto brightness = brightnesses.at(i);
                auto model_matrix = glm::rotate(
                    glm::scale(
                        glm::translate(
                            glm::mat4(1.0), glm::vec3(position.x, position.y, position.z)),
                        glm::vec3(scaling, scaling, scaling)),
                    glm::radians(rotation), glm::vec3(0, 1, 0));
                auto model_no_translation_matrix = glm::rotate(
                    glm::scale(
                        glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0)),
                        glm::vec3(scaling, scaling, scaling)),
                    glm::radians(rotation), glm::vec3(0, 1, 0));
                glUniformMatrix4fv(p->location_model_, 1, GL_FALSE, glm::value_ptr(model_matrix));
                glUniformMatrix4fv(
                    p->location_model_no_translation_, 1, GL_FALSE,
                    glm::value_ptr(model_no_translation_matrix));
                glUniform3fv(p->location_color_mod_, 1, glm::value_ptr(colorMod));
                glm::vec3 viewPos(
                    Player::Get()->GetSpaceCoord().x, Player::Get()->GetSpaceCoord().y,
                    Player::Get()->GetSpaceCoord().z);
                glUniform3fv(p->location_view_pos_, 1, glm::value_ptr(viewPos));
                glm::vec3 fogColorGl(
                    GraphicsGl::Get()->GetFogColorObjects().r,
                    GraphicsGl::Get()->GetFogColorObjects().g,
                    GraphicsGl::Get()->GetFogColorObjects().b);
                glUniform3fv(p->location_fog_color_, 1, glm::value_ptr(fogColorGl));
                glUniform1f(p->location_alpha_, brightness);
                glUniform1f(p->location_no_fog_, 0.0f);
                glUniform1f(p->location_no_lighting_, 0.0f);
                glBindVertexArray(body_data.bodyId);
                glBindTexture(GL_TEXTURE_2D, image_id);
                glDrawElements(GL_TRIANGLES, body_data.numVertices, GL_UNSIGNED_INT, NULL);
            }
        }
        glBindVertexArray(0);
        if (!p->is_batch_drawing_)
            glUseProgram(0);
    }

    void
    RendererModels::Cleanup()
    /*/////////////////////*/
    {
        CleanupBase();
    }
}