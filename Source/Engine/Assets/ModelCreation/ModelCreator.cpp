//////////////////////////////////////////////////////////////////////
#include "ModelCreator.hpp"
#include "Engine/Core/ModelStructure/Model.hpp"
#include "ModelPartCreator.hpp"
//////////////////////////////////////////////////////////////////////
using std::make_shared;
using std::map;
using std::shared_ptr;
using std::string;
using std::vector;
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    shared_ptr<Model>
    ModelCreator::CreateModel(const aiScene *scene)
    /*////////////////////////////////////////////////////////////*/
    {
        auto animDuration = scene->mNumAnimations > 0 ? scene->mAnimations[0]->mDuration : 0;
        auto newModel = make_shared<Model>(animDuration);
        for (auto &modelPart : GetModelParts(scene))
            newModel->modelParts.push_back(modelPart);
        return newModel;
    }

    auto
    ModelCreator::GetModelParts(const aiScene *scene) const -> vector<shared_ptr<ModelPart>>
    /*/////////////////////////////////////////////////////////////////////////////////////////*/
    {
        vector<shared_ptr<ModelPart>> allModelparts;
        for (auto &entry : GetModelMeshIds(scene))
        /****************************************/
        {
            string nodeName = *entry.first;
            auto nodeMeshIds = entry.second;
            for (auto &mesh : GetNodeMeshes(scene, nodeMeshIds))
            /**************************************************/
            {
                auto newModelPart =
                    ModelPartCreator::Get().CreateModelPartFromMesh(scene, nodeName, mesh);
                allModelparts.push_back(newModelPart);
            }
        }
        return allModelparts;
    }

    auto
    ModelCreator::GetModelMeshIds(const aiScene *scene) const
        -> map<shared_ptr<string>, vector<int>>
    /*//////////////////////////////////////////////////////////*/
    {
        auto rootNode = scene->mRootNode;
        auto numSubNodes = rootNode->mNumChildren;
        map<shared_ptr<string>, vector<int>> nodeNameToMeshes;
        for (auto i = 0; i < numSubNodes; i++)
        /************************************/
        {
            auto subNode = rootNode->mChildren[i];
            auto subNodeName = subNode->mName;
            auto numMeshes = subNode->mNumMeshes;
            vector<int> meshes;
            for (auto j = 0; j < numMeshes; j++)
                meshes.push_back(subNode->mMeshes[j]);
            nodeNameToMeshes.insert({make_shared<string>(subNodeName.C_Str()), meshes});
        }
        return nodeNameToMeshes;
    }

    vector<aiMesh *>
    ModelCreator::GetNodeMeshes(const aiScene *scene, vector<int> nodeMeshIds) const
    /*/////////////////////////////////////////////////////////////////////////////////////////////*/
    {
        vector<aiMesh *> nodeMeshes;
        auto numMeshes = nodeMeshIds.size();
        for (auto i = 0; i < numMeshes; i++)
        /**********************************/
        {
            auto mesh = scene->mMeshes[nodeMeshIds.at(i)];
            nodeMeshes.push_back(mesh);
        }
        return nodeMeshes;
    }
}
//////////////////////////////////////////////////////////////////////