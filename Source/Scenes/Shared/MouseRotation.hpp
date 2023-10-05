//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    enum class RotationTypes
    /*////////////////////*/ {
        None,
        CameraAndPlayer,
        OnlyCamera
    };
    
    class MouseRotation : public Singleton<MouseRotation>
    /*/////////////////////////////////////////////////*/ {
      private:
        void Update();
        void UpdateRotationInProgress();
        RotationTypes rotationType = RotationTypes::None;
        const float rotationSens = 1100.0f;
        Uint64 ticksLastUpdate = 0;
        friend class PlayScene;
        friend class EditorScene;
    };
}
//////////////////////////////////////////////////////////////////////