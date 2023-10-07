//////////////////////////////////////////////////////////////////////
#include "ModelPartCreator.hpp"
#include "Engine/Core/ModelStructure/ModelPart.hpp"
#include "ModelPartKeyframeCreator.hpp"
//////////////////////////////////////////////////////////////////////
using std::make_shared;
using std::map;
using std::shared_ptr;
using std::string;
using std::vector;
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    auto
    ModelPartCreator::CreateModelPartFromMesh(
        const aiScene *scene, string nodeName, aiMesh *mesh) const -> shared_ptr<ModelPart>
    /*///////////////////////////////////////////////////////////////////////////////////*/
    {
        auto newModelPart = make_shared<ModelPart>();
        auto texNameHash = GetTexNameHashForMesh(scene, mesh);
        newModelPart->texNameHash = texNameHash;
        auto noKeyfrmAtTime0Exists =
            ModelPartKeyframeCreator::Get().GetPositionKeyframe(scene, nodeName, 0).mTime > 0;
        auto nrOfKeyframes =
            ModelPartKeyframeCreator::Get().GetNodePositionKeyframes(scene, nodeName).size();
        for (auto k = 0; k < nrOfKeyframes; k++)
        /**************************************/
        {
            auto positionKeyframe =
                ModelPartKeyframeCreator::Get().GetPositionKeyframe(scene, nodeName, k);
            auto rotationKeyFrame =
                ModelPartKeyframeCreator::Get().GetRotationKeyframe(scene, nodeName, k);
            auto scalingKeyframe =
                ModelPartKeyframeCreator::Get().GetScalingKeyframe(scene, nodeName, k);
            auto keyframeTime = static_cast<float>(positionKeyframe.mTime);
            auto newModelPartKeyframe = GetNewModelPartKeyframe(
                scene, nodeName, mesh, positionKeyframe, rotationKeyFrame, scalingKeyframe);
            newModelPart->timeline->keyframes.insert({keyframeTime, newModelPartKeyframe});
            if (noKeyfrmAtTime0Exists && k == 0)
                newModelPart->timeline->keyframes.insert({0.0f, newModelPartKeyframe});
        }
        return newModelPart;
    }

    int
    ModelPartCreator::GetTexNameHashForMesh(const aiScene *scene, aiMesh *mesh) const
    /*/////////////////////////////////////////////////////////////////////////////*/
    {
        auto textureNameHashcodes = GetTexNameHashcodes(scene);
        auto material = mesh->mMaterialIndex;
        auto textureNameHash = textureNameHashcodes.at(material);
        return textureNameHash;
    }

    auto
    ModelPartCreator::GetNewModelPartKeyframe(
        const aiScene *scene, string nodeName, aiMesh *mesh, aiVectorKey positionKeyFrame,
        aiQuatKey rotationKeyFrame, aiVectorKey scalingKeyFrame) const
        -> shared_ptr<ModelPartKeyframe>
    /*//////////////////////////////////////////////////////////////////////////////////*/
    {
        auto newModelPartKeyframe = make_shared<ModelPartKeyframe>();
        auto nodeTransformation = GetNodeTransformation(scene, nodeName);
        auto numVertices = mesh->mNumVertices;
        for (auto i = 0; i < numVertices; i++)
        /************************************/
        {
            auto meshVertex = mesh->mVertices[i];
            auto meshNormal = mesh->mNormals[i];
            auto meshUv = mesh->mTextureCoords[0][i];
            auto position = GetPosition(
                meshVertex, nodeTransformation, positionKeyFrame, rotationKeyFrame,
                scalingKeyFrame);
            auto color = Color{0.0f, 0.0f, 0.0f, 0.0f};
            auto normal = GetPosition(
                meshNormal, nodeTransformation, positionKeyFrame, rotationKeyFrame,
                scalingKeyFrame);
            auto uv = Point2F{1.0f - meshUv.x, 1.0f - meshUv.y};
            Vertex3F animVertex;
            animVertex.position = position;
            animVertex.color = color;
            animVertex.normal = normal;
            animVertex.uv = uv;
            newModelPartKeyframe->vertices.push_back(animVertex);
        }
        return newModelPartKeyframe;
    }

    vector<int>
    ModelPartCreator::GetTexNameHashcodes(const aiScene *scene) const
    /*/////////////////////////////////////////////////////////////*/
    {
        vector<int> textureNameHashcodes;
        auto numMaterials = scene->mNumMaterials;
        for (auto i = 0; i < numMaterials; i++)
        /*************************************/
        {
            auto materialName = scene->mMaterials[i]->GetName();
            aiString textureNameCstr;
            scene->mMaterials[i]->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), textureNameCstr);
            auto textureName = string(textureNameCstr.C_Str());
            auto textureNameNoExtension = textureName.substr(0, textureName.length() - 4);
            auto textureNameHash = Hash(textureNameNoExtension);
            textureNameHashcodes.push_back(textureNameHash);
        }
        return textureNameHashcodes;
    }

    aiMatrix4x4
    ModelPartCreator::GetNodeTransformation(const aiScene *scene, string nodeName) const
    /*////////////////////////////////////////////////////////////////////////////////*/
    {
        auto allTransformations = GetTransformations(scene);
        for (auto &alpha : allTransformations)
        /************************************/
        {
            if (*alpha.first == nodeName.data())
                return alpha.second;
        }
        return aiMatrix4x4();
    }

    Point3F
    ModelPartCreator::GetPosition(
        aiVector3D vertex, aiMatrix4x4 node_transformation, aiVectorKey positionKeyFrame,
        aiQuatKey rotationKeyFrame, aiVectorKey scalingKeyFrame) const
    /*/////////////////////////////////////////////////////////////////////////////////*/
    {
        aiVector3D aiPosition = {vertex.x, vertex.y, vertex.z};
        aiPosition.x *= scalingKeyFrame.mValue.x;
        aiPosition.y *= scalingKeyFrame.mValue.y;
        aiPosition.z *= scalingKeyFrame.mValue.z;
        aiPosition = rotationKeyFrame.mValue.GetMatrix() * aiPosition;
        aiPosition = node_transformation * aiPosition;
        auto position = Point3F{aiPosition.x, aiPosition.y, aiPosition.z};
        Translate(&position, positionKeyFrame);
        aiPosition = {position.x, position.y, position.z};
        position = {aiPosition.x, aiPosition.y, aiPosition.z};
        float temp;
        temp = position.y;
        position.y = position.z;
        position.z = temp;
        return position;
    }

    auto
    ModelPartCreator::GetTransformations(const aiScene *scene) const
        -> map<shared_ptr<string>, aiMatrix4x4>
    /*////////////////////////////////////////////////////////////*/
    {
        auto rootNode = scene->mRootNode;
        auto numNodes = rootNode->mNumChildren;
        auto nodeNameToTransformations = map<shared_ptr<string>, aiMatrix4x4>();
        for (auto i = 0; i < numNodes; i++)
        /*********************************/
        {
            auto node = rootNode->mChildren[i];
            auto nodeName = node->mName;
            auto nodeTransformation = node->mTransformation;
            nodeNameToTransformations.insert(
                {make_shared<string>(nodeName.C_Str()), nodeTransformation});
        }
        return nodeNameToTransformations;
    }

    void
    ModelPartCreator::Translate(Point3F *position, aiVectorKey positionKeyFrame) const
    /*//////////////////////////////////////////////////////////////////////////////*/
    {
        auto translationMatrix = aiMatrix4x4();
        translationMatrix[0][3] = positionKeyFrame.mValue.x;
        translationMatrix[1][3] = positionKeyFrame.mValue.y;
        translationMatrix[2][3] = positionKeyFrame.mValue.z;
        translationMatrix[0][0] = 1;
        translationMatrix[1][1] = 1;
        translationMatrix[2][2] = 1;
        translationMatrix[3][3] = 1;
        auto aiPosition = aiVector3D{position->x, position->y, position->z};
        aiPosition = translationMatrix * aiPosition;
        *position = {-aiPosition.x, aiPosition.y, aiPosition.z};
    }
}
//////////////////////////////////////////////////////////////////////