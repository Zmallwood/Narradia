#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    enum class RotationTypes
    /*////////////////////*/
    {
        None,
        CameraAndPlayer,
        OnlyCamera
    };
    
    class MouseRotation : public Singleton<MouseRotation>
    /*/////////////////////////////////////////////////*/
    {
      private:
        void Update();
        void UpdateRotationInProgress();

        RotationTypes current_rotation_type_ = RotationTypes::None;
        const float rotation_sensitivity_ = 1100.0f;
        
        friend class PlayScene;
        friend class EditorScene;
        friend class MobTargeting;
    };
}