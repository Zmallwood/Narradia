#include "Camera.hpp"
#include "ConfigurationWorldDraw.hpp"
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
        auto lookAt = playerPosition3D;
        auto playerHeight = 2.0f;
        auto lookFrom = cameraPosition;
        lookAt.y += playerHeight;
        lookFrom.y += playerHeight;
        auto usedCameraDistance = cameraDistance;
        auto usedFov = fov * 7.0f / std::sqrt(usedCameraDistance);
        auto newPerspectiveMat =
            glm::perspective(glm::radians(usedFov / 2), 1600.0f / 900.0f, 0.1f, 1000.0f);
        auto newViewMat = glm::lookAt(
            glm::vec3(lookFrom.x, lookFrom.y, lookFrom.z), glm::vec3(lookAt.x, lookAt.y, lookAt.z),
            glm::vec3(0.0, 1.0, 0.0));
        Log();
        CameraGl::Get()->SetPerspectiveMatrix(newPerspectiveMat);
        Log();
        CameraGl::Get()->SetViewMatrix(newViewMat);
        if (cameraDistance == 2.0f)
            Cursor::Get()->HideThisFrame();
    }

    float
    Camera::GetZoomAmount()
    /*///////////////////*/
    {
        return cameraDistance;
    }

    void
    Camera::UpdateZooming()
    /*///////////////////*/
    {
        auto mouseWheelDelta = MouseInput::Get()->MouseWheelDeltaPickResult();
        if (mouseWheelDelta != 0)
        /***********************/
        {
            auto distanceChange = mouseWheelDelta / zoomSens;
            if (SceneManager::Get()->GetCurrentScene() == SceneNames::Editor)
                distanceChange *= 2;
            cameraDistance += distanceChange;
        }
        if (SceneManager::Get()->GetCurrentScene() == SceneNames::Play)
            cameraDistance = std::max(std::min(cameraDistance, 250.0f), 2.0f);
        else if (SceneManager::Get()->GetCurrentScene() == SceneNames::Editor)
            cameraDistance = std::max(std::min(cameraDistance, 800.0f), 2.0f);
    }

    void
    Camera::CalculateCameraPosition()
    /*/////////////////////////////*/
    {
        auto modulePlayer = Player::Get();
        auto worldMap = World::Get();
        auto usedCameraDistance = cameraDistance;
        auto usedVerticalAngle = verticalAngle;
        auto playerPosNoElev = modulePlayer->GetSpaceCoord();
        auto dzUnrotated = CosDegrees(usedVerticalAngle) * usedCameraDistance;
        auto hypotenuse = dzUnrotated;
        auto dx = SinDegrees(horizontalAngle) * hypotenuse - 3.0f * SinDegrees(horizontalAngle);
        auto dz = CosDegrees(horizontalAngle) * hypotenuse - 3.0f * CosDegrees(horizontalAngle);
        auto dy = SinDegrees(usedVerticalAngle) * usedCameraDistance;
        auto dxPlayer = -4.0f * SinDegrees(horizontalAngle);
        auto dzPlayer = -4.0f * CosDegrees(horizontalAngle);
        auto playerWorldAreaPos = modulePlayer->GetWorldAreaPos();
        Log();
        auto playerPos = modulePlayer->GetPosition();
        if (worldMap)
        /***********/
        {
            Log();
            auto mapArea = worldMap->GetMapAreaAtZLevel(playerWorldAreaPos.z);
            if (mapArea)
            /**********/
            {
                Log();
                auto playerTile = mapArea->GetTile(playerPos.ToIntPoint());
                auto elevAmount = kElevAmount;
                auto tileCoord = playerPos.ToIntPoint();
                auto tile = mapArea->GetTile(tileCoord);
                auto elev00 = static_cast<float>(tile->GetElevation());
                auto elev10 = elev00;
                auto elev11 = elev00;
                auto elev01 = elev00;
                auto coord10 = tileCoord.Translate(1, 0);
                auto coord11 = tileCoord.Translate(1, 1);
                auto coord01 = tileCoord.Translate(0, 1);
                if (MapArea::IsInsideMap(coord10))
                    elev10 = mapArea->GetTile(coord10)->GetElevation();
                if (MapArea::IsInsideMap(coord11))
                    elev11 = mapArea->GetTile(coord11)->GetElevation();
                if (MapArea::IsInsideMap(coord01))
                    elev01 = mapArea->GetTile(coord01)->GetElevation();
                auto tileAvgElev = (elev00 + elev10 + elev01 + elev11) / 4.0f;
                auto playerTileDx = playerPos.x - static_cast<int>(playerPos.x) - 0.5f;
                auto playerTileDy = playerPos.y - static_cast<int>(playerPos.y) - 0.5f;
                auto elevDx = ((elev10 - elev00) + (elev11 - elev01)) / 2.0f;
                auto elevDy = ((elev01 - elev00) + (elev11 - elev10)) / 2.0f;
                auto playerElev = tileAvgElev + playerTileDx * elevDx + playerTileDy * elevDy;
                playerPosition3D =
                    playerPosNoElev.Translate(dxPlayer, playerElev * elevAmount, dzPlayer);
                cameraPosition = playerPosNoElev.Translate(dx, dy + playerElev * elevAmount, dz);
            }
        }
    }

    Point3F
    Camera::MoveCloserToCamera(Point3F original_point, float amount)
    /*////////////////////////////////////////////////////////////*/
    {
        auto camDx = (float)cameraPosition.x - original_point.x;
        auto camDz = (float)cameraPosition.z - original_point.z;
        auto radius = std::sqrt(camDx * camDx + camDz * camDz);
        return {
            original_point.x + camDx / radius * amount, original_point.y,
            original_point.z + camDz / radius * amount};
    }
}