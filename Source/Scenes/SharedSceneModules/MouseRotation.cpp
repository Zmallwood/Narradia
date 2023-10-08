#include "MouseRotation.hpp"
#include "Engine/Core/Cursor.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
#include "Scenes/PlayScene/Modules/InteractionMenu.hpp"
#include "World/Actors/Player.hpp"
#include "WorldDrawModule/Camera.hpp"
#include "WorldDrawModule/WorldDraw.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
/****************/ {
    void
    MouseRotation::Update()
    /*///////////////////*/
    {
        if (Camera::Get()->camera_distance_ == 2.0f)
        /***************************************/
        {
            Cursor::Get()->SavePosition();
            Cursor::Get()->LockMousePos();
        }
        MouseInput ::Get()->GetRightButton().AddFiredAction(
            "StartRotateCameraAndPlayer",
            [&]
            /***********************************************/
            {
                current_rotation_type_ = RotationTypes::CameraAndPlayer;
                Cursor::Get()->SavePosition();
                Cursor::Get()->LockMousePos();
                Cursor::Get()->SetCursorType(CursorTypes::Rotating);
            },
            2, 0);
        MouseInput::Get()->GetRightButton().AddReleasedAction(
            "StopRotateCameraAndPlayer",
            [&]
            /***********************************************/
            {
                current_rotation_type_ = RotationTypes::None;
                Cursor::Get()->UnlockMousePos();
                Cursor::Get()->SetCursorType(CursorTypes::Normal);
            },
            5, 0, true);
        MouseInput::Get()->GetMiddleButton().AddFiredAction(
            "StartRotateOnlyCamera",
            [&]
            /***********************************************/
            {
                current_rotation_type_ = RotationTypes::OnlyCamera;
                Cursor::Get()->SavePosition();
                Cursor::Get()->LockMousePos();
                Cursor::Get()->SetCursorType(CursorTypes::Rotating);
            },
            14);
        MouseInput::Get()->GetMiddleButton().AddReleasedAction(
            "StopRotateOnlyCamera",
            [&]
            /***********************************************/
            {
                current_rotation_type_ = RotationTypes::None;
                Cursor::Get()->UnlockMousePos();
                Cursor::Get()->SetCursorType(CursorTypes::Normal);
            },
            5);
        UpdateRotationInProgress();
    }

    void
    MouseRotation::UpdateRotationInProgress()
    /*/////////////////////////////////////*/
    {
        if (current_rotation_type_ != RotationTypes::None || Camera::Get()->camera_distance_ == 2.0f)
        /****************************************************************************/
        {
            auto canvas_size = GetCanvasSize();
            auto motion_delta = MouseInput::Get()->GetMotionDeltaPickResults();
            auto delta_x = -static_cast<float>(motion_delta.x) * rotation_sensitivity_ / canvas_size.width;
            auto delta_y = static_cast<float>(motion_delta.y) * rotation_sensitivity_ / canvas_size.height;
            Camera::Get()->horizontal_angle_ += delta_x;
            Camera::Get()->vertical_angle_ =
                std::min(std::max(Camera::Get()->vertical_angle_ + delta_y, -5.0f), 75.0f);
            if (current_rotation_type_ == RotationTypes::CameraAndPlayer ||
                Camera::Get()->camera_distance_ == 2.0f)
                Player::Get()->SetFacingAngle(Camera::Get()->horizontal_angle_);
            Cursor::Get()->RestoreSavedPosition();
        }
    }
}