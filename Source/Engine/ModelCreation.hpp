//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Model;
    class ModelPart;
    class ModelPartKeyframe;

    class ModelCreator : public Singleton<ModelCreator>
    /*///////////////////////////////////////////////*/
    {
      public:
        auto CreateModel(const aiScene *) -> std::shared_ptr<Model>;

      private:
        auto GetModelParts(const aiScene *) const -> std::vector<std::shared_ptr<ModelPart>>;
        auto GetModelMeshIds(const aiScene *) const
            -> std::map<std::shared_ptr<std::string>, std::vector<int>>;
        auto GetNodeMeshes(const aiScene *, std::vector<int>) const -> std::vector<aiMesh *>;
    };

    class ModelPartCreator : public Singleton<ModelPartCreator>
    /*///////////////////////////////////////////////////////*/
    {
      public:
        auto CreateModelPartFromMesh(const aiScene *, std::string, aiMesh *) const
            -> std::shared_ptr<ModelPart>;

      private:
        int GetTexNameHashForMesh(const aiScene *, aiMesh *) const;
        auto GetNewModelPartKeyframe(
            const aiScene *, std::string, aiMesh *, aiVectorKey, aiQuatKey, aiVectorKey) const
            -> std::shared_ptr<ModelPartKeyframe>;
        std::vector<int> GetTexNameHashcodes(const aiScene *) const;
        aiMatrix4x4 GetNodeTransformation(const aiScene *, std::string) const;
        Point3F GetPosition(aiVector3D, aiMatrix4x4, aiVectorKey, aiQuatKey, aiVectorKey) const;
        auto GetTransformations(const aiScene *) const
            -> std::map<std::shared_ptr<std::string>, aiMatrix4x4>;
        void Translate(Point3F *, aiVectorKey) const;
    };

    class ModelPartKeyframeCreator : public Singleton<ModelPartKeyframeCreator>
    /*///////////////////////////////////////////////////////////////////////*/
    {
      public:
        aiVectorKey GetPositionKeyframe(const aiScene *, std::string, int) const;
        aiQuatKey GetRotationKeyframe(const aiScene *, std::string, int) const;
        aiVectorKey GetScalingKeyframe(const aiScene *, std::string, int) const;
        auto GetNodePositionKeyframes(const aiScene *, std::string) const
            -> std::vector<aiVectorKey>;
        auto GetNodeRotationKeyframes(const aiScene *, std::string) const -> std::vector<aiQuatKey>;
        auto GetNodeScalingKeyframes(const aiScene *, std::string) const
            -> std::vector<aiVectorKey>;

      private:
        auto GetAnimPositionKeyframes(const aiScene *) const
            -> std::map<std::string, std::vector<aiVectorKey>>;
        auto GetAnimRotationKeyframes(const aiScene *) const
            -> std::map<std::string, std::vector<aiQuatKey>>;
        auto GetAnimScalingKeyframes(const aiScene *) const
            -> std::map<std::string, std::vector<aiVectorKey>>;
    };
}
//////////////////////////////////////////////////////////////////////