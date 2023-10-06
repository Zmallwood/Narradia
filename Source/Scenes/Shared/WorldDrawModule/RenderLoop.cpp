//////////////////////////////////////////////////////////////////////
#include "RenderLoop.hpp"
#include "Camera.hpp"
#include "Configuration.hpp"
#include "Engine/Core/Graphics/Rendering/Rendering.hpp"
#include "World/MapArea.hpp"
#include "World/Object.hpp"
#include "World/ObjectBehaviourList.hpp"
#include "World/ObjectsCollection.hpp"
#include "World/Player.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
#include "Engine/Core/Graphics/Rendering/CameraGl.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    RenderLoop::RenderLoop(std::function<void()> action_)
        : action(action_)
    /*/////////////////////////////////////////////////*/
    {
    }

    void RenderLoop::operator()()
    /*/////////////////////////*/
    {
        const auto &playerWorldAreaPos = Player::Get().GetWorldAreaPos();
        const auto &playerPos = Player::Get().GetPosition();
        const auto &playerTileCoord = playerPos.ToIntPoint();
        auto mapArea = World::Get().GetMapAreaAtZLevel(playerWorldAreaPos.z);
        currMapArea = mapArea;
        auto camera = Camera::GetPointer();
        const auto zoomAmount = camera->GetZoomAmount();
        const auto elevAmount = kElevAmount;
        const auto canvasSize = GetCanvasSize();
        const auto &projMat = CameraGl::Get().GetPerspectiveMatrix();
        const auto viewPort =
            glm::vec4(0.0f, canvasSize.height / 2, canvasSize.width, canvasSize.height);
        const float camOffsetAmount = kTileSize * 10.0f;
        const auto angle = std::atan2(
            camera->cameraPosition.z - playerTileCoord.y * kTileSize,
            camera->cameraPosition.x - playerTileCoord.x * kTileSize);
        const auto extraDist = std::max(
            (float)std::sin((camera->verticalAngle) * M_PI / 180.0f) * camOffsetAmount, 0.0f);
        const float pCamDx =
            std::cos(angle) * ((camOffsetAmount + extraDist) * camera->cameraDistance / 30.0f);
        const float pCamDz =
            std::sin(angle) * ((camOffsetAmount + extraDist) * camera->cameraDistance / 30.0f);
        const Point3F usedPos = Point3F{
            playerTileCoord.x * kTileSize + pCamDx, 0.0f, playerTileCoord.y * kTileSize + pCamDz};
        const float angleRad = -(camera->horizontalAngle) * static_cast<float>(M_PI) / 180.0f;
        const auto up = glm::vec3(0.0f, 1.0f, 0.0f);
        const auto modelMat = glm::rotate(glm::mat4(1.0), angleRad, up);
        auto renderRadius = static_cast<int>(kMaxRenderRadius * 0.7f);
        for (auto y = -renderRadius; y < renderRadius; y++)
        /*************************************************/
        {
            for (auto x = -renderRadius; x < renderRadius; x++)
            /*************************************************/
            {
                currX = x;
                currY = y;
                Point2 tileCoord = {playerTileCoord.x + x, playerTileCoord.y + y};
                currTileCoord = tileCoord;
                if (!MapArea::IsInsideMap(tileCoord.x, tileCoord.y))
                    continue;
                const auto distSquared = x * x + y * y;
                if (distSquared > renderRadius * renderRadius)
                    continue;
                currDistSquared = distSquared;
                const auto tile = mapArea->GetTile(tileCoord.x, tileCoord.y);
                currTile = tile;
                const auto elev00 = static_cast<float>(tile->GetElevation());
                auto elev10 = elev00;
                auto elev11 = elev00;
                auto elev01 = elev00;
                const auto coord10 = tileCoord.Translate(1, 0);
                const auto coord11 = tileCoord.Translate(1, 1);
                const auto coord01 = tileCoord.Translate(0, 1);
                currCoord10 = coord10;
                currCoord11 = coord11;
                currCoord01 = coord01;
                auto normal00 = tile->GetNormal();
                auto normal10 = normal00;
                auto normal11 = normal00;
                auto normal01 = normal00;
                if (MapArea::IsInsideMap(coord10))
                    normal10 = mapArea->GetTile(coord10)->GetNormal();
                if (MapArea::IsInsideMap(coord11))
                    normal11 = mapArea->GetTile(coord11)->GetNormal();
                if (MapArea::IsInsideMap(coord01))
                    normal01 = mapArea->GetTile(coord01)->GetNormal();
                if (MapArea::IsInsideMap(coord10))
                    elev10 = mapArea->GetTile(coord10)->GetElevation();
                if (MapArea::IsInsideMap(coord11))
                    elev11 = mapArea->GetTile(coord11)->GetElevation();
                if (MapArea::IsInsideMap(coord01))
                    elev01 = mapArea->GetTile(coord01)->GetElevation();
                currElev00 = elev00;
                currElev10 = elev10;
                currElev11 = elev11;
                currElev01 = elev01;
                auto x0 = tileCoord.x * kTileSize;
                auto y0 = elev00 * elevAmount;
                auto z0 = tileCoord.y * kTileSize;
                auto x1 = tileCoord.x * kTileSize + kTileSize;
                auto y1 = elev10 * elevAmount;
                auto z1 = tileCoord.y * kTileSize;
                auto x2 = tileCoord.x * kTileSize + kTileSize;
                auto y2 = elev11 * elevAmount;
                auto z2 = tileCoord.y * kTileSize + kTileSize;
                auto x3 = tileCoord.x * kTileSize;
                auto y3 = elev01 * elevAmount;
                auto z3 = tileCoord.y * kTileSize + kTileSize;
                auto tileAvgElev = (elev00 + elev10 + elev01 + elev11) / 4.0f;
                currTileAvgElev = tileAvgElev;
                if (tileCoord.x == playerTileCoord.x && tileCoord.y == playerTileCoord.y)
                /***********************************************************************/
                {
                    playerElev00 = elev00;
                    playerElev10 = elev10;
                    playerElev11 = elev11;
                    playerElev01 = elev01;
                    playerTileAvgElev = tileAvgElev;
                }
                glm::vec3 cRel =
                    glm::vec3{(x0 + x2) / 2.0f - usedPos.x, 0, (z0 + z2) / 2.0f - usedPos.z};
                auto screenCoord = glm::project(cRel, modelMat, projMat, viewPort);
                if (Camera::Get().cameraDistance > 10.0f)
                /*************************************/
                {
                    if (screenCoord.x <= -0.4f * canvasSize.width ||
                        screenCoord.x >= canvasSize.width * 1.4f)
                        continue;
                }
                VertexTile vertTile;
                vertTile.v0.position = {x0, y0, z0};
                vertTile.v1.position = {x1, y1, z1};
                vertTile.v2.position = {x2, y2, z2};
                vertTile.v3.position = {x3, y3, z3};
                vertTile.v0.normal = normal00;
                vertTile.v1.normal = normal10;
                vertTile.v2.normal = normal11;
                vertTile.v3.normal = normal01;
                currVertTile = vertTile;
                bool thisTileClaimedByPlayer = false;
                bool eastTileClaimedByPlayer = false;
                bool southTileClaimedByPlayer = false;
                thisTileClaimedByPlayer = Player::Get().HasClaimedTile(tileCoord);
                if (x < MapArea::GetMapSize().width - 1)
                /**************************************/
                {
                    auto eastTile = Point2{tileCoord.x + 1, tileCoord.y};
                    eastTileClaimedByPlayer = Player::Get().HasClaimedTile(eastTile);
                }
                if (y < MapArea::GetMapSize().height - 1)
                /***************************************/
                {
                    auto southTile = Point2{tileCoord.x, tileCoord.y + 1};
                    southTileClaimedByPlayer = Player::Get().HasClaimedTile(southTile);
                }
                currThisTileClaimedByPlayer = thisTileClaimedByPlayer;
                currEastTileClaimedByPlayer = eastTileClaimedByPlayer;
                currSouthTileClaimedByPlayer = southTileClaimedByPlayer;
                bool doDrawObjects = true;
                if (doDrawObjects)
                /****************/
                {
                    for (auto &objectEntry : tile->GetObjects().list)
                    /***********************************************/
                    {
                        auto objectType = objectEntry->GetObjectType();
                        auto objectScaling = objectEntry->GetModelScaling();
                        auto objectRotation = objectEntry->GetModelRotation();
                        auto animValue = (tileCoord.x * tileCoord.y * 10 + SDL_GetTicks() * 2);
                        auto redVar = objectEntry->GetRedVariation();
                        auto greenVar = objectEntry->GetGreenVariation();
                        auto blueVar = objectEntry->GetBlueVariation();
                        bool shadowAlreadyDrawn = false;
                        if (objectType)
                        /*************/
                        {
                            std::map<int, ModelRenderParameters> *data = nullptr;
                            if ((tileCoord.x + tileCoord.y) % 2 == 0)
                                data = &data1;
                            else
                                data = &data2;
                            if (data->count(Hash("Shadow")) == 0)
                                data->insert({Hash("Shadow"), ModelRenderParameters()});
                            if (!shadowAlreadyDrawn)
                            /**********************/
                            {
                                if (distSquared < 600)
                                /********************/
                                {
                                    if (objectType != Hash("ObjectTallGrass6") &&
                                        (false == ObjectBehaviourList::Get().HasBehaviourData(
                                                      objectType) ||
                                         ObjectBehaviourList::Get().GetFlags(objectType) &
                                             (int)ObjectBehaviourFlags::NoShadow == 0))
                                    /********************************************************/
                                    {
                                        data->at(Hash("Shadow"))
                                            .positions.push_back(
                                                {x0 + kTileSize / 2,
                                                 tileAvgElev * elevAmount + 0.05f * kTileSize,
                                                 z0 + kTileSize / 2});
                                        data->at(Hash("Shadow")).rotations.push_back(0.0f);
                                        data->at(Hash("Shadow")).scalings.push_back(1.5f);
                                        data->at(Hash("Shadow")).brightnesses.push_back(1.0f);
                                        data->at(Hash("Shadow"))
                                            .colorMods.push_back({1.0f, 1.0f, 1.0f});
                                        shadowAlreadyDrawn = true;
                                    }
                                }
                            }
                            if (data->count(objectType) == 0)
                                data->insert({objectType, ModelRenderParameters()});
                            auto animValue = (tileCoord.x * tileCoord.y * 10 + SDL_GetTicks() * 2);
                            data->at(objectType)
                                .positions.push_back(
                                    {x0 + kTileSize / 2, tileAvgElev * elevAmount,
                                     z0 + kTileSize / 2});
                            data->at(objectType).rotations.push_back(objectRotation);
                            data->at(objectType).scalings.push_back(objectScaling);
                            data->at(objectType).brightnesses.push_back(1.0f);
                            data->at(objectType)
                                .colorMods.push_back(glm::vec3(redVar, greenVar, blueVar));
                        }
                    }
                }
                action();
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////