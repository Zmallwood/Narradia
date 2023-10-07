//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ModelPart;
    class ModelPartKeyframe;

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
}