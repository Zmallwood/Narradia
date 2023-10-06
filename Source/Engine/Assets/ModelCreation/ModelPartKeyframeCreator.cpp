//////////////////////////////////////////////////////////////////////
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
    aiVectorKey
    ModelPartKeyframeCreator::GetPositionKeyframe(
        const aiScene *scene, string nodeName, int keyFrameIndex) const
    /*///////////////////////////////////////////////////////////////*/
    {
        auto nodePositionKeyframes = GetNodePositionKeyframes(scene, nodeName);
        aiVectorKey positionKeyframe;
        if (keyFrameIndex < nodePositionKeyframes.size())
            positionKeyframe = nodePositionKeyframes.at(keyFrameIndex);
        else
            positionKeyframe = *(nodePositionKeyframes.end() - 1);
        return positionKeyframe;
    }

    aiQuatKey
    ModelPartKeyframeCreator::GetRotationKeyframe(
        const aiScene *scene, string nodeName, int keyFrameIndex) const
    /*///////////////////////////////////////////////////////////////*/
    {
        auto nodeRotationKeyframes = GetNodeRotationKeyframes(scene, nodeName);
        aiQuatKey rotationKeyframe;
        if (keyFrameIndex < nodeRotationKeyframes.size())
            rotationKeyframe = nodeRotationKeyframes.at(keyFrameIndex);
        else
            rotationKeyframe = *(nodeRotationKeyframes.end() - 1);
        return rotationKeyframe;
    }

    aiVectorKey
    ModelPartKeyframeCreator::GetScalingKeyframe(
        const aiScene *scene, string nodeName, int keyFrameIndex) const
    /*///////////////////////////////////////////////////////////////*/
    {
        auto nodeScalingKeyframes = GetNodeScalingKeyframes(scene, nodeName);
        aiVectorKey scalingKeyframe;
        if (keyFrameIndex < nodeScalingKeyframes.size())
            scalingKeyframe = nodeScalingKeyframes.at(keyFrameIndex);
        else
            scalingKeyframe = *(nodeScalingKeyframes.end() - 1);
        return scalingKeyframe;
    }

    auto
    ModelPartKeyframeCreator::GetNodePositionKeyframes(const aiScene *scene, string nodeName) const
        -> vector<aiVectorKey>
    /*///////////////////////////////////////////////////////////////////////////////////////////*/
    {
        auto allPositionKeyframes = GetAnimPositionKeyframes(scene);
        vector<aiVectorKey> nodePositionKeyframes;
        if (allPositionKeyframes.count(nodeName))
            nodePositionKeyframes = allPositionKeyframes.at(nodeName);
        auto numPositionKeyframes = nodePositionKeyframes.size();
        auto numMaxKeyframes = numPositionKeyframes;
        if (numMaxKeyframes == 0)
        /***********************/
        {
            numMaxKeyframes = 1;
            nodePositionKeyframes.push_back(aiVectorKey(0.0, aiVector3D()));
        }
        return nodePositionKeyframes;
    }

    auto
    ModelPartKeyframeCreator::GetNodeRotationKeyframes(const aiScene *scene, string nodeName) const
        -> vector<aiQuatKey>
    /*///////////////////////////////////////////////////////////////////////////////////////////*/
    {
        auto allRotationKeyframes = GetAnimRotationKeyframes(scene);
        vector<aiQuatKey> nodeRotationKeyframes;
        if (allRotationKeyframes.count(nodeName))
            nodeRotationKeyframes = allRotationKeyframes.at(nodeName);
        auto numRotationKeyframes = nodeRotationKeyframes.size();
        auto numMaxKeyframes = numRotationKeyframes;
        if (numMaxKeyframes == 0)
        /***********************/
        {
            numMaxKeyframes = 1;
            nodeRotationKeyframes.push_back(aiQuatKey(0.0, aiQuaternion()));
        }
        return nodeRotationKeyframes;
    }

    auto
    ModelPartKeyframeCreator::GetNodeScalingKeyframes(const aiScene *scene, string nodeName) const
        -> vector<aiVectorKey>
    /*//////////////////////////////////////////////////////////////////////////////////////////*/
    {
        auto allScalingKeyframes = GetAnimScalingKeyframes(scene);
        vector<aiVectorKey> nodeScalingKeyframes;
        if (allScalingKeyframes.count(nodeName))
            nodeScalingKeyframes = allScalingKeyframes.at(nodeName);
        auto numScalingKeyframes = nodeScalingKeyframes.size();
        auto numMaxKeyframes = numScalingKeyframes;
        if (numMaxKeyframes == 0)
        /***********************/
        {
            numMaxKeyframes = 1;
            nodeScalingKeyframes.push_back(aiVectorKey(0.0, aiVector3D(1.0)));
        }
        return nodeScalingKeyframes;
    }

    map<string, vector<aiVectorKey>>
    ModelPartKeyframeCreator::GetAnimPositionKeyframes(const aiScene *scene) const
    /*//////////////////////////////////////////////////////////////////////////*/
    {
        auto nodeNameToPositionKeyframes = map<string, vector<aiVectorKey>>();
        if (scene->mNumAnimations == 0)
        /*****************************/
        {
            auto numMeshes = scene->mNumMeshes;
            for (auto i = 0; i < numMeshes; i++)
            /**********************************/
            {
                auto mesh = scene->mMeshes[i];
                auto originalName = string(mesh->mName.C_Str());
                auto nodeName = originalName.substr(0, originalName.length() - 5);
                vector<aiVectorKey> positionKeyframes;
                aiVectorKey positionKeyframe;
                positionKeyframe.mTime = 0.0;
                positionKeyframe.mValue = aiVector3D(0.0, 0.0, 0.0);
                positionKeyframes.push_back(positionKeyframe);
                nodeNameToPositionKeyframes.insert({nodeName.c_str(), positionKeyframes});
            }
        }
        else
        /****/
        {
            auto animation0 = scene->mAnimations[0];
            auto numChannels = animation0->mNumChannels;
            for (auto i = 0; i < numChannels; i++)
            /************************************/
            {
                auto animChannel = animation0->mChannels[i];
                auto nodeName = string(animChannel->mNodeName.C_Str());
                auto numPositionKeys = animChannel->mNumPositionKeys;
                vector<aiVectorKey> positionKeyframes;
                aiVectorKey originalPositionKeyframe;
                for (auto j = 0; j < numPositionKeys; j++)
                /****************************************/
                {
                    if (j == 0)
                        originalPositionKeyframe = animChannel->mPositionKeys[j];
                    auto positionKeyframe = animChannel->mPositionKeys[j];
                    positionKeyframe.mValue -= originalPositionKeyframe.mValue;
                    positionKeyframe.mValue.z *= -1;
                    positionKeyframes.push_back(positionKeyframe);
                }
                if (numPositionKeys > 0)
                    nodeNameToPositionKeyframes.insert({nodeName, positionKeyframes});
            }
        }
        return nodeNameToPositionKeyframes;
    }

    map<string, vector<aiQuatKey>>
    ModelPartKeyframeCreator::GetAnimRotationKeyframes(const aiScene *scene) const
    /*//////////////////////////////////////////////////////////////////////////*/
    {
        auto nodeNameToRotationKeyframes = map<string, vector<aiQuatKey>>();
        if (scene->mNumAnimations == 0)
        /*****************************/
        {
            auto numMeshes = scene->mNumMeshes;
            for (auto i = 0; i < numMeshes; i++)
            /**********************************/
            {
                auto mesh = scene->mMeshes[i];
                auto originalName = string(mesh->mName.C_Str());
                auto nodeName = originalName.substr(0, originalName.length() - 5);
                vector<aiQuatKey> rotationKeyframes;
                aiQuatKey rotationKeyframe;
                rotationKeyframe.mTime = 0.0;
                rotationKeyframe.mValue = aiQuaternion(0.0, 0.0, 0.0);
                rotationKeyframes.push_back(rotationKeyframe);
                nodeNameToRotationKeyframes.insert({nodeName.c_str(), rotationKeyframes});
            }
        }
        else
        /**/
        {
            auto animation0 = scene->mAnimations[0];
            auto numChannels = animation0->mNumChannels;
            for (auto i = 0; i < numChannels; i++)
            /************************************/
            {
                auto animChannel = animation0->mChannels[i];
                auto nodeName = string(animChannel->mNodeName.C_Str());
                auto numRotationKeys = animChannel->mNumRotationKeys;
                vector<aiQuatKey> rotationKeyframes;
                aiQuatKey originalRotationKeyframe;
                for (auto j = 0; j < numRotationKeys; j++)
                /****************************************/
                {
                    if (j == 0)
                        originalRotationKeyframe = animChannel->mRotationKeys[j];
                    auto rotationKeyframe = animChannel->mRotationKeys[j];
                    rotationKeyframe.mValue.x -= originalRotationKeyframe.mValue.x;
                    rotationKeyframe.mValue.y -= originalRotationKeyframe.mValue.y;
                    rotationKeyframe.mValue.z -= originalRotationKeyframe.mValue.z;
                    rotationKeyframes.push_back(rotationKeyframe);
                }
                if (numRotationKeys > 0)
                    nodeNameToRotationKeyframes.insert({nodeName, rotationKeyframes});
            }
        }
        return nodeNameToRotationKeyframes;
    }

    map<string, vector<aiVectorKey>>
    ModelPartKeyframeCreator::GetAnimScalingKeyframes(const aiScene *scene) const
    /*/////////////////////////////////////////////////////////////////////////*/
    {
        auto nodeNameToScalingKeyframes = map<string, vector<aiVectorKey>>();
        if (scene->mNumAnimations == 0)
        /*****************************/
        {
            auto numMeshes = scene->mNumMeshes;
            for (auto i = 0; i < numMeshes; i++)
            /**********************************/
            {
                auto mesh = scene->mMeshes[i];
                auto originalName = string(mesh->mName.C_Str());
                auto nodeName = originalName.substr(0, originalName.length() - 5);
                vector<aiVectorKey> scalingKeyframes;
                aiVectorKey scalingKeyframe;
                scalingKeyframe.mTime = 0.0;
                scalingKeyframe.mValue = aiVector3D(1.0, -1.0, 1.0);
                scalingKeyframes.push_back(scalingKeyframe);
                nodeNameToScalingKeyframes.insert({nodeName.c_str(), scalingKeyframes});
            }
        }
        else
        /**/
        {
            auto animation0 = scene->mAnimations[0];
            auto numChannels = animation0->mNumChannels;
            for (auto i = 0; i < numChannels; i++)
            /************************************/
            {
                auto animChannel = animation0->mChannels[i];
                auto nodeName = string(animChannel->mNodeName.C_Str());
                auto numScalingKeys = animChannel->mNumScalingKeys;
                vector<aiVectorKey> scalingKeyframes;
                aiVectorKey originalScalingKeyframe;
                for (auto j = 0; j < numScalingKeys; j++)
                /***************************************/
                {
                    if (j == 0)
                        originalScalingKeyframe = animChannel->mScalingKeys[i];
                    auto scalingKeyframe = animChannel->mScalingKeys[j];
                    scalingKeyframe.mValue.x /= originalScalingKeyframe.mValue.x;
                    scalingKeyframe.mValue.y /= originalScalingKeyframe.mValue.y;
                    scalingKeyframe.mValue.z /= originalScalingKeyframe.mValue.z;
                    scalingKeyframes.push_back(scalingKeyframe);
                }
                if (numScalingKeys > 0)
                    nodeNameToScalingKeyframes.insert({nodeName, scalingKeyframes});
            }
        }
        return nodeNameToScalingKeyframes;
    }
}
//////////////////////////////////////////////////////////////////////