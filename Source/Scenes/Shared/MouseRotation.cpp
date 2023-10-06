//////////////////////////////////////////////////////////////////////
#include "MouseRotation.hpp"
#include "Scenes/PlayScene/Modules/InteractionMenu.hpp"
#include "World/Player.hpp"
#include "WorldDrawModule/Camera.hpp"
#include "WorldDrawModule/WorldDraw.hpp"
#include "Engine/Core/Cursor.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
/****************/ {
    void MouseRotation::Update()
    /*////////////////////////*/
    {
        if (Camera::Get().cameraDistance == 2.0f)
        /***************************************/
        {
            Cursor::Get().SavePosition();
            Cursor::Get().LockMousePos();
        }
        MouseInput ::Get().GetRightButton().AddFiredAction(
            "StartRotateCameraAndPlayer",
            [&]
            /***********************************************/
            {
                rotationType = RotationTypes::CameraAndPlayer;
                Cursor::Get().SavePosition();
                Cursor::Get().LockMousePos();
                Cursor::Get().SetCursorType(CursorTypes::Rotating);
            },
            2, 0);
        MouseInput::Get().GetRightButton().AddReleasedAction(
            "StopRotateCameraAndPlayer",
            [&]
            /***********************************************/
            {
                rotationType = RotationTypes::None;
                Cursor::Get().UnlockMousePos();
                Cursor::Get().SetCursorType(CursorTypes::Normal);
            },
            5, 0, true);
        MouseInput::Get().GetMiddleButton().AddFiredAction(
            "StartRotateOnlyCamera",
            [&]
            /***********************************************/
            {
                rotationType = RotationTypes::OnlyCamera;
                Cursor::Get().SavePosition();
                Cursor::Get().LockMousePos();
                Cursor::Get().SetCursorType(CursorTypes::Rotating);
            },
            14);
        MouseInput::Get().GetMiddleButton().AddReleasedAction(
            "StopRotateOnlyCamera",
            [&]
            /***********************************************/
            {
                rotationType = RotationTypes::None;
                Cursor::Get().UnlockMousePos();
                Cursor::Get().SetCursorType(CursorTypes::Normal);
            },
            5);
        UpdateRotationInProgress();
    }

    void MouseRotation::UpdateRotationInProgress()
    /*//////////////////////////////////////////*/
    {
        if (rotationType != RotationTypes::None || Camera::Get().cameraDistance == 2.0f)
        /****************************************************************************/
        {
            auto canvasSize = GetCanvasSize();
            auto motionDelta = MouseInput::Get().GetMotionDeltaPickResults();
            auto deltaX = -static_cast<float>(motionDelta.x) * rotationSens / canvasSize.width;
            auto deltaY = static_cast<float>(motionDelta.y) * rotationSens / canvasSize.height;
            Camera::Get().horizontalAngle += deltaX;
            Camera::Get().verticalAngle =
                std::min(std::max(Camera::Get().verticalAngle + deltaY, -5.0f), 75.0f);
            if (rotationType == RotationTypes::CameraAndPlayer ||
                Camera::Get().cameraDistance == 2.0f)
                Player::Get().SetFacingAngle(Camera::Get().horizontalAngle);
            Cursor::Get().RestoreSavedPosition();
        }
    }
}
//////////////////////////////////////////////////////////////////////
