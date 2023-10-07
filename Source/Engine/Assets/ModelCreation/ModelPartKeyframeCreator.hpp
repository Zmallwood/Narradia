//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
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