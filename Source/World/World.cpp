//////////////////////////////////////////////////////////////////////
#include "World.hpp"
#include "Engine/Core/Graphics/Rendering/RendererTiles.hpp"
#include "MapArea.hpp"
#include "Object.hpp"
#include "ObjectsCollection.hpp"
#include "Actors/Player.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/ConfigurationWorldDraw.hpp"
#include "Tile.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class World::Pimpl
    /*//////////////*/
    {
      public:
        std::map<int, std::shared_ptr<MapArea>> mapAreas;
    };

    World::World()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////////*/
    {
    }

    void
    World::AddMapAreaAtZLevel(int zLevel, std::shared_ptr<MapArea> newMapArea)
    /*///////////////////////////////////////////////////////////////////////////*/
    {
        p->mapAreas[zLevel] = newMapArea;
    }

    void
    World::RemoveMapAreaAtZLevel(int zLevel)
    /*/////////////////////////////////////////*/
    {
        if (p->mapAreas.count(zLevel))
            p->mapAreas.erase(zLevel);
    }

    MapArea *
    World::GetMapAreaAtZLevel(int zLevel)
    /*//////////////////////////////////////////*/
    {
        if (p->mapAreas.count(zLevel))
            return p->mapAreas[zLevel].get();
        return nullptr;
    }

    MapArea *
    World::GetCurrentMapArea()
    /*///////////////////////////////*/
    {
        auto playerWorldAreaPos = Player::Get().GetWorldAreaPos();
        return p->mapAreas[playerWorldAreaPos.z].get();
    }

    std::vector<MapArea *>
    World::GetAllMapAreas()
    /*//////////////////////////////////////////*/
    {
        std::vector<MapArea *> mapAreasResult;
        for (auto &mapArea : p->mapAreas)
            mapAreasResult.push_back(mapArea.second.get());
        return mapAreasResult;
    }

    void
    World::CalculateNormals(bool updateExistingTiles)
    /*//////////////////////////////////////////////////*/
    {
        auto mapAreas = World::Get().GetAllMapAreas();
        for (auto &mapArea : mapAreas)
        /****************************/
        {
            for (auto y = 0; y < MapArea::GetMapSize().height; y++)
            /*****************************************************/
            {
                for (auto x = 0; x < MapArea::GetMapSize().width; x++)
                /****************************************************/
                {
                    auto tileCoord = Point2{x, y};
                    const auto tile = mapArea->GetTile(tileCoord.x, tileCoord.y);
                    const auto elev00 = static_cast<float>(tile->GetElevation());
                    auto elev10 = elev00;
                    auto elev11 = elev00;
                    auto elev01 = elev00;
                    const auto coord10 = tileCoord.Translate(1, 0);
                    const auto coord11 = tileCoord.Translate(1, 1);
                    const auto coord01 = tileCoord.Translate(0, 1);
                    if (MapArea::IsInsideMap(coord10))
                        elev10 = mapArea->GetTile(coord10)->GetElevation();
                    if (MapArea::IsInsideMap(coord11))
                        elev11 = mapArea->GetTile(coord11)->GetElevation();
                    if (MapArea::IsInsideMap(coord01))
                        elev01 = mapArea->GetTile(coord01)->GetElevation();
                    auto x0 = tileCoord.x * kTileSize;
                    auto y0 = elev00 * kElevAmount;
                    auto z0 = tileCoord.y * kTileSize;
                    auto x1 = tileCoord.x * kTileSize + kTileSize;
                    auto y1 = elev10 * kElevAmount;
                    auto z1 = tileCoord.y * kTileSize;
                    auto x2 = tileCoord.x * kTileSize + kTileSize;
                    auto y2 = elev11 * kElevAmount;
                    auto z2 = tileCoord.y * kTileSize + kTileSize;
                    auto x3 = tileCoord.x * kTileSize;
                    auto y3 = elev01 * kElevAmount;
                    auto z3 = tileCoord.y * kTileSize + kTileSize;
                    auto p0 = Point3F{x0, y0, z0};
                    auto p1 = Point3F{x1, y1, z1};
                    auto p3 = Point3F{x3, y3, z3};
                    tile->CalculateNormal(p0, p1, p3);
                }
            }
            for (auto y = 0; y < MapArea::GetMapSize().height; y++)
            /*****************************************************/
            {
                for (auto x = 0; x < MapArea::GetMapSize().width; x++)
                /****************************************************/
                {
                    Point2 tileCoord = {x, y};
                    const auto tile = mapArea->GetTile(tileCoord.x, tileCoord.y);
                    const auto elev00 = static_cast<float>(tile->GetElevation());
                    auto elev10 = elev00;
                    auto elev11 = elev00;
                    auto elev01 = elev00;
                    const auto coord10 = tileCoord.Translate(1, 0);
                    const auto coord11 = tileCoord.Translate(1, 1);
                    const auto coord01 = tileCoord.Translate(0, 1);
                    if (MapArea::IsInsideMap(coord10))
                        elev10 = mapArea->GetTile(coord10)->GetElevation();
                    if (MapArea::IsInsideMap(coord11))
                        elev11 = mapArea->GetTile(coord11)->GetElevation();
                    if (MapArea::IsInsideMap(coord01))
                        elev01 = mapArea->GetTile(coord01)->GetElevation();
                    auto x0 = tileCoord.x * kTileSize;
                    auto y0 = elev00 * kElevAmount;
                    auto z0 = tileCoord.y * kTileSize;
                    auto x1 = tileCoord.x * kTileSize + kTileSize;
                    auto y1 = elev10 * kElevAmount;
                    auto z1 = tileCoord.y * kTileSize;
                    auto x2 = tileCoord.x * kTileSize + kTileSize;
                    auto y2 = elev11 * kElevAmount;
                    auto z2 = tileCoord.y * kTileSize + kTileSize;
                    auto x3 = tileCoord.x * kTileSize;
                    auto y3 = elev01 * kElevAmount;
                    auto z3 = tileCoord.y * kTileSize + kTileSize;
                    Vertex3F v0;
                    Vertex3F v1;
                    Vertex3F v2;
                    Vertex3F v3;
                    v0.position = {x0, y0, z0};
                    v1.position = {x1, y1, z1};
                    v2.position = {x2, y2, z2};
                    v3.position = {x3, y3, z3};
                    v0.uv = {0.0f, 0.0f};
                    v1.uv = {1.0f, 0.0f};
                    v2.uv = {1.0f, 1.0f};
                    v3.uv = {0.0f, 1.0f};
                    auto rVar = tile->GetRedVariation();
                    auto gVar = tile->GetGreenVariation();
                    auto bVar = tile->GetBlueVariation();
                    auto rVarE = rVar;
                    auto gVarE = gVar;
                    auto bVarE = bVar;
                    auto rVarS = rVar;
                    auto gVarS = gVar;
                    auto bVarS = bVar;
                    auto rVarSE = rVar;
                    auto gVarSE = gVar;
                    auto bVarSE = bVar;
                    if (tileCoord.x < MapArea::GetMapSize().width - 1)
                    /************************************************/
                    {
                        rVarE = mapArea->GetTile({tileCoord.x + 1, tileCoord.y})->GetRedVariation();
                        gVarE =
                            mapArea->GetTile({tileCoord.x + 1, tileCoord.y})->GetGreenVariation();
                        bVarE =
                            mapArea->GetTile({tileCoord.x + 1, tileCoord.y})->GetBlueVariation();
                    }
                    if (tileCoord.y < MapArea::GetMapSize().height - 1)
                    /*************************************************/
                    {
                        rVarS = mapArea->GetTile({tileCoord.x, tileCoord.y + 1})->GetRedVariation();
                        gVarS =
                            mapArea->GetTile({tileCoord.x, tileCoord.y + 1})->GetGreenVariation();
                        bVarS =
                            mapArea->GetTile({tileCoord.x, tileCoord.y + 1})->GetBlueVariation();
                    }
                    if (tileCoord.x < MapArea::GetMapSize().width - 1 &&
                        tileCoord.y < MapArea::GetMapSize().height - 1)
                    /**************************************************/
                    {
                        rVarSE =
                            mapArea->GetTile({tileCoord.x + 1, tileCoord.y + 1})->GetRedVariation();
                        gVarSE = mapArea->GetTile({tileCoord.x + 1, tileCoord.y + 1})
                                     ->GetGreenVariation();
                        bVarSE = mapArea->GetTile({tileCoord.x + 1, tileCoord.y + 1})
                                     ->GetBlueVariation();
                    }
                    if (tile->GetGroundType() == Hash("GroundWater"))
                    /***********************************************/
                    {
                        rVar = rVarE = rVarS = rVarSE = 0.0f;
                        gVar = gVarE = gVarS = gVarSE = 0.0f;
                        bVar = bVarE = bVarS = bVarSE = 0.0f;
                    }
                    v0.color = {1.0f - rVar, 1.0f - gVar, 1.0f - bVar, 1.0f};
                    v1.color = {1.0f - rVarE, 1.0f - gVarE, 1.0f - bVarE, 1.0f};
                    v2.color = {1.0f - rVarSE, 1.0f - gVarSE, 1.0f - bVarSE, 1.0f};
                    v3.color = {1.0f - rVarS, 1.0f - gVarS, 1.0f - bVarS, 1.0f};
                    tile->SetVertex0(v0);
                    tile->SetVertex1(v1);
                    tile->SetVertex2(v2);
                    tile->SetVertex3(v3);
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
                    GLuint tileRendId;
                    if (false == updateExistingTiles)
                    /*******************************/
                    {
                        tileRendId = RendererTiles::Get().NewTile();
                    }
                    else
                    /**/
                    {
                        tileRendId = tile->GetTileRendId();
                    }
                    RendererTiles::Get().SetGeometryTile(
                        tileRendId, v0, v1, v2, v3, normal00, normal10, normal11, normal01);
                    tile->SetTileRendId(tileRendId);
                }
            }
        }
    }

    void
    World::ResetColorVariations()
    /*//////////////////////////////*/
    {
        auto mapAreas = World::Get().GetAllMapAreas();
        for (auto &mapArea : mapAreas)
        /****************************/
        {
            for (auto y = 0; y < MapArea::GetMapSize().height; y++)
            /*****************************************************/
            {
                for (auto x = 0; x < MapArea::GetMapSize().width; x++)
                /****************************************************/
                {
                    mapArea->GetTile({x, y})->SetRedVariation(0.0f);
                    mapArea->GetTile({x, y})->SetGreenVariation(0.0f);
                    mapArea->GetTile({x, y})->SetBlueVariation(0.0f);
                }
            }
        }
    }

    void
    World::ApplyDefaultColorVariations()
    /*/////////////////////////////////////*/
    {
        auto mapAreas = World::Get().GetAllMapAreas();
        for (auto &mapArea : mapAreas)
        /****************************/
        {
            for (auto y = 0; y < MapArea::GetMapSize().height; y++)
            /*****************************************************/
            {
                for (auto x = 0; x < MapArea::GetMapSize().width; x++)
                /****************************************************/
                {
                    if (mapArea->GetTile({x, y})->GetGroundType() == Hash("GroundRock"))
                    /******************************************************************/
                    {
                        if (mapArea->GetTile({x, y})->GetRedVariation() < 0.3f)
                            mapArea->GetTile({x, y})->AlterRedVariation(0.3f);
                        if (mapArea->GetTile({x, y})->GetGreenVariation() < 0.5f)
                            mapArea->GetTile({x, y})->AlterGreenVariation(0.5f);
                    }
                    else if (mapArea->GetTile({x, y})->GetGroundType() == Hash("GroundRiver"))
                    /**************************************************************************/
                    {
                        if (mapArea->GetTile({x, y})->GetRedVariation() < 0.5f)
                            mapArea->GetTile({x, y})->AlterRedVariation(0.5f);
                        if (mapArea->GetTile({x, y})->GetGreenVariation() < 0.3f)
                            mapArea->GetTile({x, y})->AlterGreenVariation(0.3f);
                        if (mapArea->GetTile({x, y})->GetBlueVariation() > -1.5f)
                            mapArea->GetTile({x, y})->AlterBlueVariation(-1.5f);
                    }
                    else if (mapArea->GetTile({x, y})->GetGroundType() == Hash("GroundGrass"))
                    /**************************************************************************/
                    {
                        if (mapArea->GetTile({x, y})->GetRedVariation() < 0.6f)
                            mapArea->GetTile({x, y})->AlterRedVariation(0.6f);
                        if (mapArea->GetTile({x, y})->GetGreenVariation() < 0.3f)
                            mapArea->GetTile({x, y})->AlterGreenVariation(0.3f);
                        if (mapArea->GetTile({x, y})->GetBlueVariation() < 0.6f)
                            mapArea->GetTile({x, y})->AlterBlueVariation(0.6f);
                    }
                }
            }
        }
    }

    void
    World::GenerateRandomColorVariations()
    /*///////////////////////////////////////*/
    {
        auto mapAreas = World::Get().GetAllMapAreas();
        for (auto &mapArea : mapAreas)
        /****************************/
        {
            for (auto i = 0; i < 10000; i++)
            /******************************/
            {
                auto xCenter = rand() % MapArea::GetMapSize().width;
                auto yCenter = rand() % MapArea::GetMapSize().height;
                auto r = 3 + rand() % 10;
                auto rVar = ((float)rand()) / RAND_MAX * 0.2f;
                auto gVar = ((float)rand()) / RAND_MAX * 0.4f;
                auto bVar = ((float)rand()) / RAND_MAX * 0.2f;
                for (auto y = yCenter - r; y <= yCenter + r; y++)
                /***********************************************/
                {
                    for (auto x = xCenter - r; x <= xCenter + r; x++)
                    /***********************************************/
                    {
                        if (x < 0 || y < 0 || x >= MapArea::GetMapSize().width ||
                            y >= MapArea::GetMapSize().height)
                            continue;
                        if (mapArea->GetTile({x, y})->GetGroundType() == Hash("GroundWater"))
                            continue;
                        auto dx = x - xCenter;
                        auto dy = y - yCenter;
                        if (dx * dx + dy * dy < r * r)
                        /****************************/
                        {
                            if (mapArea->GetTile({x, y})->GetGroundType() != Hash("GroundRock"))
                            /******************************************************************/
                            {
                                mapArea->GetTile({x, y})->SetRedVariation(rVar);
                                mapArea->GetTile({x, y})->SetGreenVariation(gVar);
                                mapArea->GetTile({x, y})->SetBlueVariation(bVar);
                            }
                            else
                            /**/
                            {
                                mapArea->GetTile({x, y})->SetRedVariation(rVar / 3.0f);
                                mapArea->GetTile({x, y})->SetGreenVariation(gVar / 3.0f);
                                mapArea->GetTile({x, y})->SetBlueVariation(bVar / 3.0f);
                            }
                            for (auto &object : mapArea->GetTile(x, y)->GetObjects().list)
                            /************************************************************/
                            {
                                auto rVarObj = 0.8f + ((float)rand()) / RAND_MAX * 0.2f;
                                auto gVarObj = 0.8f + ((float)rand()) / RAND_MAX * 0.2f;
                                auto bVarObj = 0.8f + ((float)rand()) / RAND_MAX * 0.2f;
                                object->SetRedVariation(rVarObj);
                                object->SetGreenVariation(gVarObj);
                                object->SetBlueVariation(bVarObj);
                            }
                        }
                    }
                }
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////