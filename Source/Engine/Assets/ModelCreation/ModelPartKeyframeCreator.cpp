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
    aiVectorKey ModelPartKeyframeCreator::GetPositionKeyframe(
        const aiScene *scene, string nodeName, int keyFrameIndex) const {
        auto node_position_keyframes = GetNodePositionKeyframes(scene, nodeName);
        aiVectorKey position_keyframe;
        if (keyFrameIndex < node_position_keyframes.size())
            position_keyframe = node_position_keyframes.at(keyFrameIndex);
        else
            position_keyframe = *(node_position_keyframes.end() - 1);
        return position_keyframe;
    }

    aiQuatKey ModelPartKeyframeCreator::GetRotationKeyframe(
        const aiScene *scene, string nodeName, int keyFrameIndex) const {
        auto node_rotation_keyframes = GetNodeRotationKeyframes(scene, nodeName);
        aiQuatKey rotation_keyframe;
        if (keyFrameIndex < node_rotation_keyframes.size())
            rotation_keyframe = node_rotation_keyframes.at(keyFrameIndex);
        else
            rotation_keyframe = *(node_rotation_keyframes.end() - 1);
        return rotation_keyframe;
    }

    aiVectorKey ModelPartKeyframeCreator::GetScalingKeyframe(
        const aiScene *scene, string nodeName, int keyFrameIndex) const {
        auto node_scaling_keyframes = GetNodeScalingKeyframes(scene, nodeName);
        aiVectorKey scaling_keyframe;
        if (keyFrameIndex < node_scaling_keyframes.size())
            scaling_keyframe = node_scaling_keyframes.at(keyFrameIndex);
        else
            scaling_keyframe = *(node_scaling_keyframes.end() - 1);
        return scaling_keyframe;
    }

    auto
    ModelPartKeyframeCreator::GetNodePositionKeyframes(const aiScene *scene, string nodeName) const
        -> vector<aiVectorKey> {
        auto all_position_keyframes = GetAnimPositionKeyframes(scene);
        vector<aiVectorKey> node_position_keyframes;
        if (all_position_keyframes.count(nodeName))
            node_position_keyframes = all_position_keyframes.at(nodeName);
        auto num_position_keyframes = node_position_keyframes.size();
        auto num_max_keyframes = num_position_keyframes;
        if (num_max_keyframes == 0) {
            num_max_keyframes = 1;
            node_position_keyframes.push_back(aiVectorKey(0.0, aiVector3D()));
        }
        return node_position_keyframes;
    }

    auto
    ModelPartKeyframeCreator::GetNodeRotationKeyframes(const aiScene *scene, string nodeName) const
        -> vector<aiQuatKey> {
        auto all_rotation_keyframes = GetAnimRotationKeyframes(scene);
        vector<aiQuatKey> node_rotation_keyframes;
        if (all_rotation_keyframes.count(nodeName))
            node_rotation_keyframes = all_rotation_keyframes.at(nodeName);
        auto num_rotation_keyframes = node_rotation_keyframes.size();
        auto num_max_keyframes = num_rotation_keyframes;
        if (num_max_keyframes == 0) {
            num_max_keyframes = 1;
            node_rotation_keyframes.push_back(aiQuatKey(0.0, aiQuaternion()));
        }
        return node_rotation_keyframes;
    }

    auto
    ModelPartKeyframeCreator::GetNodeScalingKeyframes(const aiScene *scene, string nodeName) const
        -> vector<aiVectorKey> {
        auto all_scaling_keyframes = GetAnimScalingKeyframes(scene);
        vector<aiVectorKey> node_scaling_keyframes;
        if (all_scaling_keyframes.count(nodeName))
            node_scaling_keyframes = all_scaling_keyframes.at(nodeName);
        auto num_scaling_keyframes = node_scaling_keyframes.size();
        auto num_max_keyframes = num_scaling_keyframes;
        if (num_max_keyframes == 0) {
            num_max_keyframes = 1;
            node_scaling_keyframes.push_back(aiVectorKey(0.0, aiVector3D(1.0)));
        }
        return node_scaling_keyframes;
    }

    map<string, vector<aiVectorKey>>
    ModelPartKeyframeCreator::GetAnimPositionKeyframes(const aiScene *scene) const {
        auto node_name_to_position_keyframes = map<string, vector<aiVectorKey>>();
        if (scene->mNumAnimations == 0) {
            auto num_meshes = scene->mNumMeshes;
            for (auto i = 0; i < num_meshes; i++) {
                auto mesh = scene->mMeshes[i];
                auto original_name = string(mesh->mName.C_Str());
                auto node_name = original_name.substr(0, original_name.length() - 5);
                vector<aiVectorKey> position_keyframes;
                aiVectorKey position_keyframe;
                position_keyframe.mTime = 0.0;
                position_keyframe.mValue = aiVector3D(0.0, 0.0, 0.0);
                position_keyframes.push_back(position_keyframe);
                node_name_to_position_keyframes.insert({node_name.c_str(), position_keyframes});
            }
        }
        else {
            auto animation0 = scene->mAnimations[0];
            auto num_channels = animation0->mNumChannels;
            for (auto i = 0; i < num_channels; i++) {
                auto anim_channel = animation0->mChannels[i];
                auto node_name = string(anim_channel->mNodeName.C_Str());
                auto num_position_keys = anim_channel->mNumPositionKeys;
                vector<aiVectorKey> position_keyframes;
                aiVectorKey original_position_keyframe;
                for (auto j = 0; j < num_position_keys; j++) {
                    if (j == 0)
                        original_position_keyframe = anim_channel->mPositionKeys[j];
                    auto position_keyframe = anim_channel->mPositionKeys[j];
                    position_keyframe.mValue -= original_position_keyframe.mValue;
                    position_keyframe.mValue.z *= -1;
                    position_keyframes.push_back(position_keyframe);
                }
                if (num_position_keys > 0)
                    node_name_to_position_keyframes.insert({node_name, position_keyframes});
            }
        }
        return node_name_to_position_keyframes;
    }

    map<string, vector<aiQuatKey>>
    ModelPartKeyframeCreator::GetAnimRotationKeyframes(const aiScene *scene) const {
        auto node_name_to_rotation_keyframes = map<string, vector<aiQuatKey>>();
        if (scene->mNumAnimations == 0) {
            auto num_meshes = scene->mNumMeshes;
            for (auto i = 0; i < num_meshes; i++) {
                auto mesh = scene->mMeshes[i];
                auto original_name = string(mesh->mName.C_Str());
                auto node_name = original_name.substr(0, original_name.length() - 5);
                vector<aiQuatKey> rotation_keyframes;
                aiQuatKey rotation_keyframe;
                rotation_keyframe.mTime = 0.0;
                rotation_keyframe.mValue = aiQuaternion(0.0, 0.0, 0.0);
                rotation_keyframes.push_back(rotation_keyframe);
                node_name_to_rotation_keyframes.insert({node_name.c_str(), rotation_keyframes});
            }
        }
        else {
            auto animation0 = scene->mAnimations[0];
            auto num_channels = animation0->mNumChannels;
            for (auto i = 0; i < num_channels; i++) {
                auto anim_channel = animation0->mChannels[i];
                auto node_name = string(anim_channel->mNodeName.C_Str());
                auto num_rotation_keys = anim_channel->mNumRotationKeys;
                vector<aiQuatKey> rotation_keyframes;
                aiQuatKey original_rotation_keyframe;
                for (auto j = 0; j < num_rotation_keys; j++) {
                    if (j == 0)
                        original_rotation_keyframe = anim_channel->mRotationKeys[j];
                    auto rotation_keyframe = anim_channel->mRotationKeys[j];
                    rotation_keyframe.mValue.x -= original_rotation_keyframe.mValue.x;
                    rotation_keyframe.mValue.y -= original_rotation_keyframe.mValue.y;
                    rotation_keyframe.mValue.z -= original_rotation_keyframe.mValue.z;
                    rotation_keyframes.push_back(rotation_keyframe);
                }
                if (num_rotation_keys > 0)
                    node_name_to_rotation_keyframes.insert({node_name, rotation_keyframes});
            }
        }
        return node_name_to_rotation_keyframes;
    }

    map<string, vector<aiVectorKey>>
    ModelPartKeyframeCreator::GetAnimScalingKeyframes(const aiScene *scene) const {
        auto node_name_to_scaling_keyframes = map<string, vector<aiVectorKey>>();
        if (scene->mNumAnimations == 0) {
            auto num_meshes = scene->mNumMeshes;
            for (auto i = 0; i < num_meshes; i++) {
                auto mesh = scene->mMeshes[i];
                auto original_name = string(mesh->mName.C_Str());
                auto node_name = original_name.substr(0, original_name.length() - 5);
                vector<aiVectorKey> scaling_keyframes;
                aiVectorKey scaling_keyframe;
                scaling_keyframe.mTime = 0.0;
                scaling_keyframe.mValue = aiVector3D(1.0, -1.0, 1.0);
                scaling_keyframes.push_back(scaling_keyframe);
                node_name_to_scaling_keyframes.insert({node_name.c_str(), scaling_keyframes});
            }
        }
        else {
            auto animation0 = scene->mAnimations[0];
            auto num_channels = animation0->mNumChannels;
            for (auto i = 0; i < num_channels; i++) {
                auto anim_channel = animation0->mChannels[i];
                auto node_name = string(anim_channel->mNodeName.C_Str());
                auto num_scaling_keys = anim_channel->mNumScalingKeys;
                vector<aiVectorKey> scaling_keyframes;
                aiVectorKey original_scaling_keyframe;
                for (auto j = 0; j < num_scaling_keys; j++) {
                    if (j == 0)
                        original_scaling_keyframe = anim_channel->mScalingKeys[i];
                    auto scaling_keyframe = anim_channel->mScalingKeys[j];
                    scaling_keyframe.mValue.x /= original_scaling_keyframe.mValue.x;
                    scaling_keyframe.mValue.y /= original_scaling_keyframe.mValue.y;
                    scaling_keyframe.mValue.z /= original_scaling_keyframe.mValue.z;
                    scaling_keyframes.push_back(scaling_keyframe);
                }
                if (num_scaling_keys > 0)
                    node_name_to_scaling_keyframes.insert({node_name, scaling_keyframes});
            }
        }
        return node_name_to_scaling_keyframes;
    }
}