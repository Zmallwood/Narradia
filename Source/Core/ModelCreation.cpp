//////////////////////////////////////////////////////////////////////
#include "ModelCreation.hpp"
//////////////////////////////////////////////////////////////////////
using std::make_shared;
using std::map;
using std::shared_ptr;
using std::string;
using std::vector;
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    shared_ptr<Model> ModelCreator::CreateModel(const aiScene *scene)
    /*////////////////////////////////////////////////////////////*/ {
        auto animDuration = scene->mNumAnimations > 0 ? scene->mAnimations[0]->mDuration : 0;
        auto newModel = make_shared<Model>(animDuration);
        for (auto &modelPart : GetModelParts(scene))
            newModel->modelParts.push_back(modelPart);
        return newModel;
    } // Function

    auto ModelCreator::GetModelParts(const aiScene *scene) const -> vector<shared_ptr<ModelPart>>
    /*/////////////////////////////////////////////////////////////////////////////////////////*/ {
        vector<shared_ptr<ModelPart>> allModelparts;
        for (auto &entry : GetModelMeshIds(scene))
        /****************************************/ {
            string nodeName = *entry.first;
            auto nodeMeshIds = entry.second;
            for (auto &mesh : GetNodeMeshes(scene, nodeMeshIds))
            /**************************************************/ {
                auto newModelPart =
                    ModelPartCreator::Get().CreateModelPartFromMesh(scene, nodeName, mesh);
                allModelparts.push_back(newModelPart);
            }
        }
        return allModelparts;
    } // Function

    auto ModelCreator::GetModelMeshIds(const aiScene *scene) const
        -> map<shared_ptr<string>, vector<int>>
    /*//////////////////////////////////////////////////////////*/
    {
        auto rootNode = scene->mRootNode;
        auto numSubNodes = rootNode->mNumChildren;
        map<shared_ptr<string>, vector<int>> nodeNameToMeshes;
        for (auto i = 0; i < numSubNodes; i++)
        /************************************/ {
            auto subNode = rootNode->mChildren[i];
            auto subNodeName = subNode->mName;
            auto numMeshes = subNode->mNumMeshes;
            vector<int> meshes;
            for (auto j = 0; j < numMeshes; j++)
                meshes.push_back(subNode->mMeshes[j]);
            nodeNameToMeshes.insert({make_shared<string>(subNodeName.C_Str()), meshes});
        }
        return nodeNameToMeshes;
    } // Function

    vector<aiMesh *>
    ModelCreator::GetNodeMeshes(const aiScene *scene, vector<int> nodeMeshIds) const
    /*/////////////////////////////////////////////////////////////////////////////////////////////*/
    {
        vector<aiMesh *> nodeMeshes;
        auto numMeshes = nodeMeshIds.size();
        for (auto i = 0; i < numMeshes; i++)
        /**********************************/ {
            auto mesh = scene->mMeshes[nodeMeshIds.at(i)];
            nodeMeshes.push_back(mesh);
        }
        return nodeMeshes;
    } // Function

    auto ModelPartCreator::CreateModelPartFromMesh(
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
        /**************************************/ {
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
    } // Function

    int ModelPartCreator::GetTexNameHashForMesh(const aiScene *scene, aiMesh *mesh) const
    /*/////////////////////////////////////////////////////////////////////////////////*/ {
        auto textureNameHashcodes = GetTexNameHashcodes(scene);
        auto material = mesh->mMaterialIndex;
        auto textureNameHash = textureNameHashcodes.at(material);
        return textureNameHash;
    } // Function

    auto ModelPartCreator::GetNewModelPartKeyframe(
        const aiScene *scene, string nodeName, aiMesh *mesh, aiVectorKey positionKeyFrame,
        aiQuatKey rotationKeyFrame, aiVectorKey scalingKeyFrame) const
        -> shared_ptr<ModelPartKeyframe>
    /*//////////////////////////////////////////////////////////////////////////////////*/
    {
        auto newModelPartKeyframe = make_shared<ModelPartKeyframe>();
        auto nodeTransformation = GetNodeTransformation(scene, nodeName);
        auto numVertices = mesh->mNumVertices;
        for (auto i = 0; i < numVertices; i++)
        /************************************/ {
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
    } // Function

    vector<int> ModelPartCreator::GetTexNameHashcodes(const aiScene *scene) const
    /*/////////////////////////////////////////////////////////////////////////*/ {
        vector<int> textureNameHashcodes;
        auto numMaterials = scene->mNumMaterials;
        for (auto i = 0; i < numMaterials; i++)
        /*************************************/ {
            auto materialName = scene->mMaterials[i]->GetName();
            aiString textureNameCstr;
            scene->mMaterials[i]->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), textureNameCstr);
            auto textureName = string(textureNameCstr.C_Str());
            auto textureNameNoExtension = textureName.substr(0, textureName.length() - 4);
            auto textureNameHash = Hash(textureNameNoExtension);
            textureNameHashcodes.push_back(textureNameHash);
        }
        return textureNameHashcodes;
    } // Function

    aiMatrix4x4 ModelPartCreator::GetNodeTransformation(const aiScene *scene, string nodeName) const
    /*////////////////////////////////////////////////////////////////////////////////////////////*/
    {
        auto allTransformations = GetTransformations(scene);
        for (auto &alpha : allTransformations)
        /************************************/ {
            if (*alpha.first == nodeName.data()) return alpha.second;
        }
        return aiMatrix4x4();
    } // Function

    Point3F ModelPartCreator::GetPosition(
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
    } // Function

    auto ModelPartCreator::GetTransformations(const aiScene *scene) const
        -> map<shared_ptr<string>, aiMatrix4x4>
    /*/////////////////////////////////////////////////////////////////*/
    {
        auto rootNode = scene->mRootNode;
        auto numNodes = rootNode->mNumChildren;
        auto nodeNameToTransformations = map<shared_ptr<string>, aiMatrix4x4>();
        for (auto i = 0; i < numNodes; i++)
        /*********************************/ {
            auto node = rootNode->mChildren[i];
            auto nodeName = node->mName;
            auto nodeTransformation = node->mTransformation;
            nodeNameToTransformations.insert(
                {make_shared<string>(nodeName.C_Str()), nodeTransformation});
        }
        return nodeNameToTransformations;
    } // Function

    void ModelPartCreator::Translate(Point3F *position, aiVectorKey positionKeyFrame) const
    /*///////////////////////////////////////////////////////////////////////////////////*/ {
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
    } // Function

    aiVectorKey ModelPartKeyframeCreator::GetPositionKeyframe(
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
    } // Function

    aiQuatKey ModelPartKeyframeCreator::GetRotationKeyframe(
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
    } // Function

    aiVectorKey ModelPartKeyframeCreator::GetScalingKeyframe(
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
    } // Function

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
        /***********************/ {
            numMaxKeyframes = 1;
            nodePositionKeyframes.push_back(aiVectorKey(0.0, aiVector3D()));
        }
        return nodePositionKeyframes;
    } // Function

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
        /***********************/ {
            numMaxKeyframes = 1;
            nodeRotationKeyframes.push_back(aiQuatKey(0.0, aiQuaternion()));
        }
        return nodeRotationKeyframes;
    } // Function

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
        /***********************/ {
            numMaxKeyframes = 1;
            nodeScalingKeyframes.push_back(aiVectorKey(0.0, aiVector3D(1.0)));
        }
        return nodeScalingKeyframes;
    } // Function

    map<string, vector<aiVectorKey>>
    ModelPartKeyframeCreator::GetAnimPositionKeyframes(const aiScene *scene) const
    /*//////////////////////////////////////////////////////////////////////////*/ {
        auto nodeNameToPositionKeyframes = map<string, vector<aiVectorKey>>();
        if (scene->mNumAnimations == 0)
        /*****************************/ {
            auto numMeshes = scene->mNumMeshes;
            for (auto i = 0; i < numMeshes; i++)
            /**********************************/ {
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
        } else
        /****/ {
            auto animation0 = scene->mAnimations[0];
            auto numChannels = animation0->mNumChannels;
            for (auto i = 0; i < numChannels; i++)
            /************************************/ {
                auto animChannel = animation0->mChannels[i];
                auto nodeName = string(animChannel->mNodeName.C_Str());
                auto numPositionKeys = animChannel->mNumPositionKeys;
                vector<aiVectorKey> positionKeyframes;
                aiVectorKey originalPositionKeyframe;
                for (auto j = 0; j < numPositionKeys; j++)
                /****************************************/ {
                    if (j == 0) originalPositionKeyframe = animChannel->mPositionKeys[j];
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
    } // Function

    map<string, vector<aiQuatKey>>
    ModelPartKeyframeCreator::GetAnimRotationKeyframes(const aiScene *scene) const
    /*//////////////////////////////////////////////////////////////////////////*/ {
        auto nodeNameToRotationKeyframes = map<string, vector<aiQuatKey>>();
        if (scene->mNumAnimations == 0)
        /*****************************/ {
            auto numMeshes = scene->mNumMeshes;
            for (auto i = 0; i < numMeshes; i++)
            /**********************************/ {
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
        } else
        /****/ {
            auto animation0 = scene->mAnimations[0];
            auto numChannels = animation0->mNumChannels;
            for (auto i = 0; i < numChannels; i++)
            /************************************/ {
                auto animChannel = animation0->mChannels[i];
                auto nodeName = string(animChannel->mNodeName.C_Str());
                auto numRotationKeys = animChannel->mNumRotationKeys;
                vector<aiQuatKey> rotationKeyframes;
                aiQuatKey originalRotationKeyframe;
                for (auto j = 0; j < numRotationKeys; j++)
                /****************************************/ {
                    if (j == 0) originalRotationKeyframe = animChannel->mRotationKeys[j];
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
    } // Function

    map<string, vector<aiVectorKey>>
    ModelPartKeyframeCreator::GetAnimScalingKeyframes(const aiScene *scene) const
    /*/////////////////////////////////////////////////////////////////////////*/ {
        auto nodeNameToScalingKeyframes = map<string, vector<aiVectorKey>>();
        if (scene->mNumAnimations == 0)
        /*****************************/ {
            auto numMeshes = scene->mNumMeshes;
            for (auto i = 0; i < numMeshes; i++)
            /**********************************/ {
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
        } else
        /****/ {
            auto animation0 = scene->mAnimations[0];
            auto numChannels = animation0->mNumChannels;
            for (auto i = 0; i < numChannels; i++)
            /************************************/ {
                auto animChannel = animation0->mChannels[i];
                auto nodeName = string(animChannel->mNodeName.C_Str());
                auto numScalingKeys = animChannel->mNumScalingKeys;
                vector<aiVectorKey> scalingKeyframes;
                aiVectorKey originalScalingKeyframe;
                for (auto j = 0; j < numScalingKeys; j++)
                /***************************************/ {
                    if (j == 0) originalScalingKeyframe = animChannel->mScalingKeys[i];
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
    } // Function
}
//////////////////////////////////////////////////////////////////////