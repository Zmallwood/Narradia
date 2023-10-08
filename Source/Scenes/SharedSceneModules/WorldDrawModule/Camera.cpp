#include "Camera.hpp"
#include "ConfigWorldDraw.hpp"
#include "Engine/Core/Cursor.hpp"
#include "Engine/Core/Graphics/Rendering/CameraGl.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
#include "Engine/Core/SceneManager.hpp"
#include "World/MapArea.hpp"
#include "World/Actors/Player.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
#include "WorldDraw.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    Camera::Camera()
    /*////////////*/
    {
        Log();
        CalculateCameraPosition();
    }

    void
    Camera::Update()
    /*////////////*/
    {
        Log();
        UpdateZooming();
        Log();
        CalculateCameraPosition();
        auto look_at = player_position_3d_;
        auto player_height = 2.0f;
        auto look_from = camera_position_;
        look_at.y += player_height;
        look_from.y += player_height;
        auto used_camera_distance = camera_distance_;
        auto used_fov = fov_ * 7.0f / std::sqrt(used_camera_distance);
        auto new_perspective_matrix =
            glm::perspective(glm::radians(used_fov / 2), 1600.0f / 900.0f, 0.1f, 1000.0f);
        auto new_view_matrix = glm::lookAt(
            glm::vec3(look_from.x, look_from.y, look_from.z), glm::vec3(look_at.x, look_at.y, look_at.z),
            glm::vec3(0.0, 1.0, 0.0));
        Log();
        CameraGl::Get()->SetPerspectiveMatrix(new_perspective_matrix);
        Log();
        CameraGl::Get()->SetViewMatrix(new_view_matrix);
        if (camera_distance_ == 2.0f)
            Cursor::Get()->HideThisFrame();
    }

    float
    Camera::GetZoomAmount()
    /*///////////////////*/
    {
        return camera_distance_;
    }

    void
    Camera::UpdateZooming()
    /*///////////////////*/
    {
        auto mouse_wheel_delta = MouseInput::Get()->MouseWheelDeltaPickResult();
        if (mouse_wheel_delta != 0)
        /***********************/
        {
            auto distance_change = mouse_wheel_delta / zoom_sensitivity_;
            if (SceneManager::Get()->GetCurrentScene() == SceneNames::Editor)
                distance_change *= 2;
            camera_distance_ += distance_change;
        }
        if (SceneManager::Get()->GetCurrentScene() == SceneNames::Play)
            camera_distance_ = std::max(std::min(camera_distance_, 250.0f), 2.0f);
        else if (SceneManager::Get()->GetCurrentScene() == SceneNames::Editor)
            camera_distance_ = std::max(std::min(camera_distance_, 800.0f), 2.0f);
    }

    void
    Camera::CalculateCameraPosition()
    /*/////////////////////////////*/
    {
        auto player = Player::Get();
        auto world = World::Get();
        auto userd_camera_distance = camera_distance_;
        auto used_vertical_angle = vertical_angle_;
        auto player_position_no_elevation = player->GetSpaceCoord();
        auto dz_unrotated = CosDegrees(used_vertical_angle) * userd_camera_distance;
        auto hypotenuse = dz_unrotated;
        auto dx = SinDegrees(horizontal_angle_) * hypotenuse - 3.0f * SinDegrees(horizontal_angle_);
        auto dz = CosDegrees(horizontal_angle_) * hypotenuse - 3.0f * CosDegrees(horizontal_angle_);
        auto dy = SinDegrees(used_vertical_angle) * userd_camera_distance;
        auto dx_player = -4.0f * SinDegrees(horizontal_angle_);
        auto dz_player = -4.0f * CosDegrees(horizontal_angle_);
        auto player_world_area_position = player->GetWorldAreaPos();
        Log();
        auto player_position = player->GetPosition();
        if (world)
        /***********/
        {
            Log();
            auto map_area = world->GetMapAreaAtZLevel(player_world_area_position.z);
            if (map_area)
            /**********/
            {
                Log();
                auto player_tile = map_area->GetTile(player_position.ToIntPoint());
                auto elev_amount = kElevAmount;
                auto tile_coord = player_position.ToIntPoint();
                auto tile = map_area->GetTile(tile_coord);
                auto elev00 = static_cast<float>(tile->GetElevation());
                auto elev10 = elev00;
                auto elev11 = elev00;
                auto elev01 = elev00;
                auto coord10 = tile_coord.Translate(1, 0);
                auto coord11 = tile_coord.Translate(1, 1);
                auto coord01 = tile_coord.Translate(0, 1);
                if (MapArea::IsInsideMap(coord10))
                    elev10 = map_area->GetTile(coord10)->GetElevation();
                if (MapArea::IsInsideMap(coord11))
                    elev11 = map_area->GetTile(coord11)->GetElevation();
                if (MapArea::IsInsideMap(coord01))
                    elev01 = map_area->GetTile(coord01)->GetElevation();
                auto tile_avg_elev = (elev00 + elev10 + elev01 + elev11) / 4.0f;
                auto player_tile_dx = player_position.x - static_cast<int>(player_position.x) - 0.5f;
                auto player_tile_dy = player_position.y - static_cast<int>(player_position.y) - 0.5f;
                auto elev_dx = ((elev10 - elev00) + (elev11 - elev01)) / 2.0f;
                auto elev_dy = ((elev01 - elev00) + (elev11 - elev10)) / 2.0f;
                auto player_elev = tile_avg_elev + player_tile_dx * elev_dx + player_tile_dy * elev_dy;
                player_position_3d_ =
                    player_position_no_elevation.Translate(dx_player, player_elev * elev_amount, dz_player);
                camera_position_ = player_position_no_elevation.Translate(dx, dy + player_elev * elev_amount, dz);
            }
        }
    }

    Point3F
    Camera::MoveCloserToCamera(Point3F original_point, float amount)
    /*////////////////////////////////////////////////////////////*/
    {
        auto camera_dx = (float)camera_position_.x - original_point.x;
        auto camera_dz = (float)camera_position_.z - original_point.z;
        auto radius = std::sqrt(camera_dx * camera_dx + camera_dz * camera_dz);
        return {
            original_point.x + camera_dx / radius * amount, original_point.y,
            original_point.z + camera_dz / radius * amount};
    }
}