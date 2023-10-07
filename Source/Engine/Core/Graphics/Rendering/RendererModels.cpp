#include "RendererModels.hpp"
#include "CameraGl.hpp"
#include "Engine/Assets/ImageBank.hpp"
#include "Engine/Assets/ModelBank.hpp"
#include "Engine/Core/Graphics/GraphicsGl.hpp"
#include "Engine/Core/ModelStructure/Model.hpp"
#include "ShaderProgram.hpp"
#include "World/Actors/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class RendererModels::Pimpl
    /*///////////////////////*/
    {
      public:
        struct BodyData
        /*///////////*/
        {
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
    /*//////////////////////////////*/
    {
        const GLchar *vertexShaderSource =
#include "Shaders/ModelsVertex.glsl"
            ;
        const GLchar *fragmentShaderSource =
#include "Shaders/ModelsFragment.glsl"
            ;
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

    void
    RendererModels::NewModel(int modelNameHash)
    /*///////////////////////////////////////*/
    {
        auto model = ModelBank::Get()->GetModel(modelNameHash);
        p->modelIds.insert(
            {modelNameHash, std::map<int, std::map<float, const Pimpl::BodyData>>()});
        auto iBody = 0;
        for (auto &body : model->modelParts)
        /**********************************/
        {
            auto &timelines = body->timeline->keyframes;
            for (auto &keyframe : timelines)
            /******************************/
            {
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
                    body->texNameHash, keyframeTime, bodyKeyframeId, vertices, normals);
            }
            iBody++;
        }
    }

    RenderId
    RendererModels::NewBodyKeyframe(int modelNameHash, float msTime, int numVertices)
    /*/////////////////////////////////////////////////////////////////////////////*/
    {
        auto vaoId = GenerateNewVertexArrayId();
        if (p->timelines.count(modelNameHash) == 0)
            p->timelines.insert({modelNameHash, std::map<float, RenderId>()});
        p->timelines.at(modelNameHash).insert({msTime, vaoId});
        return vaoId;
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
            p->locationProjection, 1, GL_FALSE, value_ptr(CameraGl::Get()->GetPerspectiveMatrix()));
        glUniformMatrix4fv(
            p->locationView, 1, GL_FALSE, value_ptr(CameraGl::Get()->GetViewMatrix()));
        glUniform1f(p->locationAlpha, 1.0f);
        auto imageId = ImageBank::Get()->GetImage(imageNamehash);
        glBindTexture(GL_TEXTURE_2D, imageId);
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

    void
    RendererModels::StartBatchDrawing()
    /*///////////////////////////////*/
    {
        p->isBatchDrawing = true;
        glEnable(GL_DEPTH_TEST);
        glUseProgram(GetShaderProgram()->GetProgramId());
        glUniformMatrix4fv(
            p->locationProjection, 1, GL_FALSE, value_ptr(CameraGl::Get()->GetPerspectiveMatrix()));
        glUniformMatrix4fv(
            p->locationView, 1, GL_FALSE, glm::value_ptr(CameraGl::Get()->GetViewMatrix()));
    }

    void
    RendererModels::StopBatchDrawing()
    /*//////////////////////////////*/
    {
        p->isBatchDrawing = false;
    }

    void
    RendererModels::DrawModel(
        int modelNameHash, float msTime, Point3F position, float rotation, float scaling,
        float brightness, glm::vec3 colorMod, bool noFog, bool noLighting) const
    /*/////////////////////////////////////////////////////////////////////////////////*/
    {
        if (p->modelIds.count(modelNameHash) == 0)
            return;
        if (!p->isBatchDrawing)
        /*********************/
        {
            glEnable(GL_DEPTH_TEST);
            glUseProgram(GetShaderProgram()->GetProgramId());
            glUniformMatrix4fv(
                p->locationProjection, 1, GL_FALSE,
                value_ptr(CameraGl::Get()->GetPerspectiveMatrix()));
            auto &viewMatrix = CameraGl::Get()->GetViewMatrix();
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
            Player::Get()->GetSpaceCoord().x, Player::Get()->GetSpaceCoord().y,
            Player::Get()->GetSpaceCoord().z);
        glUniform3fv(p->locationViewPos, 1, glm::value_ptr(viewPos));
        glm::vec3 fogColorGl(
            GraphicsGl::Get()->GetFogColorObjects().r, GraphicsGl::Get()->GetFogColorObjects().g,
            GraphicsGl::Get()->GetFogColorObjects().b);
        glUniform3fv(p->locationFogColor, 1, glm::value_ptr(fogColorGl));
        glUniform1f(p->locationAlpha, brightness);
        glUniform1f(p->locationNoFog, noFog ? 1.0f : 0.0f);
        glUniform1f(p->locationNoLighting, noLighting ? 1.0f : 0.0f);
        auto &allNodes = p->modelIds.at(modelNameHash);
        auto pModel = ModelBank::Get()->GetModel(modelNameHash);
        int msTimeUsed;
        if (pModel->animDuration == 0)
            msTimeUsed = 0;
        else
            msTimeUsed = static_cast<int>(msTime * p->globalAnimSpeed) % pModel->animDuration;
        auto &modelData = allNodes;
        for (auto &entry : allNodes)
        /**************************/
        {
            auto &timeline = entry.second;
            auto foundTime = -1.0f;
            const Pimpl::BodyData *pBodyData = nullptr;
            for (auto &keyframe : timeline)
            /*****************************/
            {
                auto time = keyframe.first;
                if (msTimeUsed >= time)
                    foundTime = time;
            }
            auto &bodyData = timeline.at(foundTime);
            glBindVertexArray(bodyData.bodyId);
            auto imageId = ImageBank::Get()->GetImage(bodyData.imageNameHash);
            glBindTexture(GL_TEXTURE_2D, imageId);
            glDrawElements(GL_TRIANGLES, bodyData.numVertices, GL_UNSIGNED_INT, NULL);
        }
        glBindVertexArray(0);
        if (!p->isBatchDrawing)
            glUseProgram(0);
    }

    void
    RendererModels::DrawModelsMany(
        int modelNameHash, float msTime, std::vector<Point3F> positions,
        std::vector<float> rotations, std::vector<float> scalings, std::vector<float> brightnesses,
        std::vector<glm::vec3> colorMods) const
    /*///////////////////////////////////////////////////////////////////////////////////////////*/
    {
        if (p->modelIds.count(modelNameHash) == 0)
            return;
        if (!p->isBatchDrawing)
        /*********************/
        {
            glEnable(GL_DEPTH_TEST);
            glUseProgram(GetShaderProgram()->GetProgramId());
            glUniformMatrix4fv(
                p->locationProjection, 1, GL_FALSE,
                value_ptr(CameraGl::Get()->GetPerspectiveMatrix()));
            auto &viewMatrix = CameraGl::Get()->GetViewMatrix();
            glUniformMatrix4fv(p->locationView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        }
        auto &allNodes = p->modelIds.at(modelNameHash);
        auto pModel = ModelBank::Get()->GetModel(modelNameHash);
        int msTimeUsed;
        if (pModel->animDuration == 0)
            msTimeUsed = 0;
        else
            msTimeUsed = static_cast<int>(msTime * p->globalAnimSpeed) % pModel->animDuration;
        auto &modelData = allNodes;
        for (auto &entry : allNodes)
        /**************************/
        {
            auto &timeline = entry.second;
            auto foundTime = -1.0f;
            const Pimpl::BodyData *pBodyData = nullptr;
            for (auto &keyframe : timeline)
            /*****************************/
            {
                auto time = keyframe.first;
                if (msTimeUsed >= time)
                    foundTime = time;
            }
            auto &bodyData = timeline.at(foundTime);
            auto imageId = ImageBank::Get()->GetImage(bodyData.imageNameHash);
            for (auto i = 0; i < positions.size(); i++)
            /*****************************************/
            {
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
                    Player::Get()->GetSpaceCoord().x, Player::Get()->GetSpaceCoord().y,
                    Player::Get()->GetSpaceCoord().z);
                glUniform3fv(p->locationViewPos, 1, glm::value_ptr(viewPos));
                glm::vec3 fogColorGl(
                    GraphicsGl::Get()->GetFogColorObjects().r,
                    GraphicsGl::Get()->GetFogColorObjects().g,
                    GraphicsGl::Get()->GetFogColorObjects().b);
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
        if (!p->isBatchDrawing)
            glUseProgram(0);
    }

    void
    RendererModels::Cleanup()
    /*/////////////////////*/
    {
        CleanupBase();
    }
}