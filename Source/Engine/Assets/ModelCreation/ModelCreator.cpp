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
    shared_ptr<Model> ModelCreator::CreateModel(const aiScene *scene) {
        auto anim_duration = scene->mNumAnimations > 0 ? scene->mAnimations[0]->mDuration : 0;
        auto new_model = make_shared<Model>(anim_duration);
        for (auto &model_part : GetModelParts(scene))
            new_model->modelParts.push_back(model_part);
        return new_model;
    }

    auto ModelCreator::GetModelParts(const aiScene *scene) const -> vector<shared_ptr<ModelPart>> {
        vector<shared_ptr<ModelPart>> all_model_parts;
        for (auto &entry : GetModelMeshIds(scene)) {
            string node_name = *entry.first;
            auto node_mesh_ids = entry.second;
            for (auto &mesh : GetNodeMeshes(scene, node_mesh_ids)) {
                auto new_model_part =
                    ModelPartCreator::Get()->CreateModelPartFromMesh(scene, node_name, mesh);
                all_model_parts.push_back(new_model_part);
            }
        }
        return all_model_parts;
    }

    auto ModelCreator::GetModelMeshIds(const aiScene *scene) const
        -> map<shared_ptr<string>, vector<int>> {
        auto root_node = scene->mRootNode;
        auto num_sub_nodes = root_node->mNumChildren;
        map<shared_ptr<string>, vector<int>> node_name_to_meshes;
        for (auto i = 0; i < num_sub_nodes; i++) {
            auto sub_node = root_node->mChildren[i];
            auto sub_node_name = sub_node->mName;
            auto num_meshes = sub_node->mNumMeshes;
            vector<int> meshes;
            for (auto j = 0; j < num_meshes; j++)
                meshes.push_back(sub_node->mMeshes[j]);
            node_name_to_meshes.insert({make_shared<string>(sub_node_name.C_Str()), meshes});
        }
        return node_name_to_meshes;
    }

    vector<aiMesh *>
    ModelCreator::GetNodeMeshes(const aiScene *scene, vector<int> nodeMeshIds) const {
        vector<aiMesh *> node_meshes;
        auto num_meshes = nodeMeshIds.size();
        for (auto i = 0; i < num_meshes; i++) {
            auto mesh = scene->mMeshes[nodeMeshIds.at(i)];
            node_meshes.push_back(mesh);
        }
        return node_meshes;
    }
}