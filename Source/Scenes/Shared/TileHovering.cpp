//////////////////////////////////////////////////////////////////////
#include "TileHovering.hpp"
#include "Core/Rendering.hpp"
#include "Scenes/Shared/WorldDrawModule/Configuration.hpp"
#include "World/MapArea.hpp"
#include "World/Object.hpp"
#include "World/ObjectBehaviourList.hpp"
#include "World/Player.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
#include "WorldDrawModule/Camera.hpp"
#include "WorldDrawModule/WorldDraw.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    TileHovering::TileHovering()
    /*////////////////////////*/
    {
        glIdTextHoveredObject = TextRenderer::Get().NewString();
    }

    void TileHovering::Update()
    /*///////////////////////*/
    {
        if (Camera::Get().cameraDistance == 2.0f)
        /*************************************/
        {
            hoveredTile = {-1, -1};
            return;
        }
        hoveredObjectHash = 0;
        auto &viewMat = CameraGl::Get().GetViewMatrix();
        auto &projMat = CameraGl::Get().GetPerspectiveMatrix();
        auto mousePos = GetMousePositionF();
        auto canvasSize = GetCanvasSize();
        auto playerX = Player::Get().GetPosition().x;
        auto playerY = Player::Get().GetPosition().y;
        auto tileSize = kTileSize;
        auto elevAmount = kElevAmount;
        auto playerWorldAreaPos = Player::Get().GetWorldAreaPos();
        auto *mapArea = World::Get().GetMapAreaAtZLevel(playerWorldAreaPos.z);
        auto mouseWorldNearplane = glm::unProject(
            glm::vec3(mousePos.x * canvasSize.width, (1.0f - mousePos.y) * canvasSize.height, 0.0f),
            viewMat, projMat, glm::ivec4(0, 0, canvasSize.width, canvasSize.height));
        auto mouseWorldFarplane = glm::unProject(
            glm::vec3(mousePos.x * canvasSize.width, (1.0f - mousePos.y) * canvasSize.height, 1.0f),
            viewMat, projMat, glm::ivec4(0, 0, canvasSize.width, canvasSize.height));
        auto columnsCount = 111;
        auto rowsCount = 111;
        auto playerXMajor = static_cast<int>(playerX);
        auto playerYMajor = static_cast<int>(playerY);
        bool tileFound = false;
        auto iterationFunc = [&](int x, int y) -> bool
        /********************************************/
        {
            auto mapX = playerXMajor + x;
            auto mapY = playerYMajor + y;
            if (!MapArea::IsInsideMap({mapX, mapY}))
                return false;
            auto tileCoord = Point2{mapX, mapY};
            auto tile = mapArea->GetTile(tileCoord);
            auto elev00 = static_cast<float>(tile->GetElevation());
            auto elev10 = elev00;
            auto elev11 = elev00;
            auto elev01 = elev00;
            auto tileAvgElev = (elev00 + elev10 + elev01 + elev11) / 4.0f;
            auto coord10 = tileCoord.Translate(1, 0);
            auto coord11 = tileCoord.Translate(1, 1);
            auto coord01 = tileCoord.Translate(0, 1);
            if (MapArea::IsInsideMap(coord10))
                elev10 = mapArea->GetTile(coord10)->GetElevation();
            if (MapArea::IsInsideMap(coord11))
                elev11 = mapArea->GetTile(coord11)->GetElevation();
            if (MapArea::IsInsideMap(coord01))
                elev01 = mapArea->GetTile(coord01)->GetElevation();
            auto x0 = tileCoord.x * tileSize;
            auto y0 = elev00 * elevAmount;
            auto z0 = tileCoord.y * tileSize;
            auto x2 = tileCoord.x * tileSize + tileSize;
            auto y2 = elev11 * elevAmount;
            auto z2 = tileCoord.y * tileSize + tileSize;
            auto center = glm::vec3{(x0 + x2) / 2, (y0 + y2) / 2, (z0 + z2) / 2};
            auto closestPoint =
                glm::closestPointOnLine(center, mouseWorldNearplane, mouseWorldFarplane);
            if (glm::distance(center, closestPoint) < tileSize / 2)
            /*****************************************************/
            {
                hoveredTile = {mapX, mapY};
                if (tile->GetObjectsCount() > 0)
                    hoveredObjectHash = tile->GetObjectAt(0)->GetObjectType();
                tileFound = true;
                return true;
            }
            return false;
        };
        for (int y = -(rowsCount - 1) / 2; y < (rowsCount - 1) / 2 && !tileFound; y++)
        /*****************************************************************************/
        {
            for (int x = -(columnsCount - 1) / 2; x < (columnsCount - 1) / 2 && !tileFound; x++)
            /******************************************************************************/
            {
                if (iterationFunc(x, y))
                    return;
            }
        }
    }

    void TileHovering::Render()
    /*///////////////////////*/
    {
        if (hoveredObjectHash == 0)
            return;
        if (false == ObjectBehaviourList::Get().HasBehaviourData(hoveredObjectHash))
            return;
        TextRenderer::Get().DrawString(
            glIdTextHoveredObject, ObjectBehaviourList::Get().GetLabel(hoveredObjectHash).data(),
            GetMousePositionF().Translate(0.02f, 0.01f));
    }
}
//////////////////////////////////////////////////////////////////////