#include "World.hpp"
#include "Actors/Player.hpp"
#include "Engine/Core/Graphics/Rendering/RendererTiles.hpp"
#include "MapArea.hpp"
#include "Object.hpp"
#include "ObjectsCollection.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/ConfigWorldDraw.hpp"
#include "Tile.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class World::Pimpl
    /*//////////////*/
    {
      public:
        std::map<int, std::shared_ptr<MapArea>> map_areas_;
    };

    World::World()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void
    World::AddMapAreaAtZLevel(int zLevel, std::shared_ptr<MapArea> newMapArea)
    /*//////////////////////////////////////////////////////////////////////*/
    {
        p->map_areas_[zLevel] = newMapArea;
    }

    void
    World::RemoveMapAreaAtZLevel(int zLevel)
    /*////////////////////////////////////*/
    {
        if (p->map_areas_.count(zLevel))
            p->map_areas_.erase(zLevel);
    }

    MapArea *
    World::GetMapAreaAtZLevel(int zLevel)
    /*/////////////////////////////////*/
    {
        if (p->map_areas_.count(zLevel))
            return p->map_areas_[zLevel].get();
        return nullptr;
    }

    MapArea *
    World::GetCurrentMapArea()
    /*//////////////////////*/
    {
        auto player_world_area_position = Player::Get()->GetWorldAreaPos();
        return p->map_areas_[player_world_area_position.z].get();
    }

    std::vector<MapArea *>
    World::GetAllMapAreas()
    /*///////////////////*/
    {
        std::vector<MapArea *> map_areas_result;
        for (auto &map_area : p->map_areas_)
            map_areas_result.push_back(map_area.second.get());
        return map_areas_result;
    }

    void
    World::CalculateNormals(bool updateExistingTiles)
    /*/////////////////////////////////////////////*/
    {
        auto map_areas = World::Get()->GetAllMapAreas();
        for (auto &map_area : map_areas)
        /****************************/
        {
            for (auto y = 0; y < MapArea::GetMapSize().height; y++)
            /*****************************************************/
            {
                for (auto x = 0; x < MapArea::GetMapSize().width; x++)
                /****************************************************/
                {
                    auto tile_coord = Point2{x, y};
                    const auto tile = map_area->GetTile(tile_coord.x, tile_coord.y);
                    const auto elev00 = static_cast<float>(tile->GetElevation());
                    auto elev10 = elev00;
                    auto elev11 = elev00;
                    auto elev01 = elev00;
                    const auto coord10 = tile_coord.Translate(1, 0);
                    const auto coord11 = tile_coord.Translate(1, 1);
                    const auto coord01 = tile_coord.Translate(0, 1);
                    if (MapArea::IsInsideMap(coord10))
                        elev10 = map_area->GetTile(coord10)->GetElevation();
                    if (MapArea::IsInsideMap(coord11))
                        elev11 = map_area->GetTile(coord11)->GetElevation();
                    if (MapArea::IsInsideMap(coord01))
                        elev01 = map_area->GetTile(coord01)->GetElevation();
                    auto x0 = tile_coord.x * kTileSize;
                    auto y0 = elev00 * kElevAmount;
                    auto z0 = tile_coord.y * kTileSize;
                    auto x1 = tile_coord.x * kTileSize + kTileSize;
                    auto y1 = elev10 * kElevAmount;
                    auto z1 = tile_coord.y * kTileSize;
                    auto x2 = tile_coord.x * kTileSize + kTileSize;
                    auto y2 = elev11 * kElevAmount;
                    auto z2 = tile_coord.y * kTileSize + kTileSize;
                    auto x3 = tile_coord.x * kTileSize;
                    auto y3 = elev01 * kElevAmount;
                    auto z3 = tile_coord.y * kTileSize + kTileSize;
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
                    Point2 tile_coord = {x, y};
                    const auto tile = map_area->GetTile(tile_coord.x, tile_coord.y);
                    const auto elev00 = static_cast<float>(tile->GetElevation());
                    auto elev10 = elev00;
                    auto elev11 = elev00;
                    auto elev01 = elev00;
                    const auto coord10 = tile_coord.Translate(1, 0);
                    const auto coord11 = tile_coord.Translate(1, 1);
                    const auto coord01 = tile_coord.Translate(0, 1);
                    if (MapArea::IsInsideMap(coord10))
                        elev10 = map_area->GetTile(coord10)->GetElevation();
                    if (MapArea::IsInsideMap(coord11))
                        elev11 = map_area->GetTile(coord11)->GetElevation();
                    if (MapArea::IsInsideMap(coord01))
                        elev01 = map_area->GetTile(coord01)->GetElevation();
                    auto x0 = tile_coord.x * kTileSize;
                    auto y0 = elev00 * kElevAmount;
                    auto z0 = tile_coord.y * kTileSize;
                    auto x1 = tile_coord.x * kTileSize + kTileSize;
                    auto y1 = elev10 * kElevAmount;
                    auto z1 = tile_coord.y * kTileSize;
                    auto x2 = tile_coord.x * kTileSize + kTileSize;
                    auto y2 = elev11 * kElevAmount;
                    auto z2 = tile_coord.y * kTileSize + kTileSize;
                    auto x3 = tile_coord.x * kTileSize;
                    auto y3 = elev01 * kElevAmount;
                    auto z3 = tile_coord.y * kTileSize + kTileSize;
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
                    auto red_variation = tile->GetRedVariation();
                    auto green_variation = tile->GetGreenVariation();
                    auto blue_variation = tile->GetBlueVariation();
                    auto red_variation_east = red_variation;
                    auto green_variation_east = green_variation;
                    auto blue_variation_east = blue_variation;
                    auto red_variation_south = red_variation;
                    auto green_variation_south = green_variation;
                    auto blue_variation_south = blue_variation;
                    auto red_variation_south_east = red_variation;
                    auto green_variation_south_east = green_variation;
                    auto blue_variation_south_east = blue_variation;
                    if (tile_coord.x < MapArea::GetMapSize().width - 1)
                    /************************************************/
                    {
                        red_variation_east = map_area->GetTile({tile_coord.x + 1, tile_coord.y})->GetRedVariation();
                        green_variation_east =
                            map_area->GetTile({tile_coord.x + 1, tile_coord.y})->GetGreenVariation();
                        blue_variation_east =
                            map_area->GetTile({tile_coord.x + 1, tile_coord.y})->GetBlueVariation();
                    }
                    if (tile_coord.y < MapArea::GetMapSize().height - 1)
                    /*************************************************/
                    {
                        red_variation_south = map_area->GetTile({tile_coord.x, tile_coord.y + 1})->GetRedVariation();
                        green_variation_south =
                            map_area->GetTile({tile_coord.x, tile_coord.y + 1})->GetGreenVariation();
                        blue_variation_south =
                            map_area->GetTile({tile_coord.x, tile_coord.y + 1})->GetBlueVariation();
                    }
                    if (tile_coord.x < MapArea::GetMapSize().width - 1 &&
                        tile_coord.y < MapArea::GetMapSize().height - 1)
                    /**************************************************/
                    {
                        red_variation_south_east =
                            map_area->GetTile({tile_coord.x + 1, tile_coord.y + 1})->GetRedVariation();
                        green_variation_south_east = map_area->GetTile({tile_coord.x + 1, tile_coord.y + 1})
                                     ->GetGreenVariation();
                        blue_variation_south_east = map_area->GetTile({tile_coord.x + 1, tile_coord.y + 1})
                                     ->GetBlueVariation();
                    }
                    if (tile->GetGroundType() == Hash("GroundWater"))
                    /***********************************************/
                    {
                        red_variation = red_variation_east = red_variation_south = red_variation_south_east = 0.0f;
                        green_variation = green_variation_east = green_variation_south = green_variation_south_east = 0.0f;
                        blue_variation = blue_variation_east = blue_variation_south = blue_variation_south_east = 0.0f;
                    }
                    v0.color = {1.0f - red_variation, 1.0f - green_variation, 1.0f - blue_variation, 1.0f};
                    v1.color = {1.0f - red_variation_east, 1.0f - green_variation_east, 1.0f - blue_variation_east, 1.0f};
                    v2.color = {1.0f - red_variation_south_east, 1.0f - green_variation_south_east, 1.0f - blue_variation_south_east, 1.0f};
                    v3.color = {1.0f - red_variation_south, 1.0f - green_variation_south, 1.0f - blue_variation_south, 1.0f};
                    tile->SetVertex0(v0);
                    tile->SetVertex1(v1);
                    tile->SetVertex2(v2);
                    tile->SetVertex3(v3);
                    auto normal00 = tile->GetNormal();
                    auto normal10 = normal00;
                    auto normal11 = normal00;
                    auto normal01 = normal00;
                    if (MapArea::IsInsideMap(coord10))
                        normal10 = map_area->GetTile(coord10)->GetNormal();
                    if (MapArea::IsInsideMap(coord11))
                        normal11 = map_area->GetTile(coord11)->GetNormal();
                    if (MapArea::IsInsideMap(coord01))
                        normal01 = map_area->GetTile(coord01)->GetNormal();
                    GLuint rendid_ground;
                    if (false == updateExistingTiles)
                    /*******************************/
                    {
                        rendid_ground = RendererTiles::Get()->NewTile();
                    }
                    else
                    /**/
                    {
                        rendid_ground = tile->GetTileRendId();
                    }
                    RendererTiles::Get()->SetGeometryTile(
                        rendid_ground, v0, v1, v2, v3, normal00, normal10, normal11, normal01);
                    tile->SetTileRendId(rendid_ground);
                }
            }
        }
    }

    void
    World::ResetColorVariations()
    /*/////////////////////////*/
    {
        auto map_areas = World::Get()->GetAllMapAreas();
        for (auto &map_area : map_areas)
        /****************************/
        {
            for (auto y = 0; y < MapArea::GetMapSize().height; y++)
            /*****************************************************/
            {
                for (auto x = 0; x < MapArea::GetMapSize().width; x++)
                /****************************************************/
                {
                    map_area->GetTile({x, y})->SetRedVariation(0.0f);
                    map_area->GetTile({x, y})->SetGreenVariation(0.0f);
                    map_area->GetTile({x, y})->SetBlueVariation(0.0f);
                }
            }
        }
    }

    void
    World::ApplyDefaultColorVariations()
    /*////////////////////////////////*/
    {
        auto map_areas = World::Get()->GetAllMapAreas();
        for (auto &map_area : map_areas)
        /****************************/
        {
            for (auto y = 0; y < MapArea::GetMapSize().height; y++)
            /*****************************************************/
            {
                for (auto x = 0; x < MapArea::GetMapSize().width; x++)
                /****************************************************/
                {
                    if (map_area->GetTile({x, y})->GetGroundType() == Hash("GroundRock"))
                    /******************************************************************/
                    {
                        if (map_area->GetTile({x, y})->GetRedVariation() < 0.3f)
                            map_area->GetTile({x, y})->AlterRedVariation(0.3f);
                        if (map_area->GetTile({x, y})->GetGreenVariation() < 0.5f)
                            map_area->GetTile({x, y})->AlterGreenVariation(0.5f);
                    }
                    else if (map_area->GetTile({x, y})->GetGroundType() == Hash("GroundRiver"))
                    /**************************************************************************/
                    {
                        if (map_area->GetTile({x, y})->GetRedVariation() < 0.5f)
                            map_area->GetTile({x, y})->AlterRedVariation(0.5f);
                        if (map_area->GetTile({x, y})->GetGreenVariation() < 0.3f)
                            map_area->GetTile({x, y})->AlterGreenVariation(0.3f);
                        if (map_area->GetTile({x, y})->GetBlueVariation() > -1.5f)
                            map_area->GetTile({x, y})->AlterBlueVariation(-1.5f);
                    }
                    else if (map_area->GetTile({x, y})->GetGroundType() == Hash("GroundGrass"))
                    /**************************************************************************/
                    {
                        if (map_area->GetTile({x, y})->GetRedVariation() < 0.6f)
                            map_area->GetTile({x, y})->AlterRedVariation(0.6f);
                        if (map_area->GetTile({x, y})->GetGreenVariation() < 0.3f)
                            map_area->GetTile({x, y})->AlterGreenVariation(0.3f);
                        if (map_area->GetTile({x, y})->GetBlueVariation() < 0.6f)
                            map_area->GetTile({x, y})->AlterBlueVariation(0.6f);
                    }
                }
            }
        }
    }

    void
    World::GenerateRandomColorVariations()
    /*//////////////////////////////////*/
    {
        auto map_areas = World::Get()->GetAllMapAreas();
        for (auto &map_area : map_areas)
        /****************************/
        {
            for (auto i = 0; i < 10000; i++)
            /******************************/
            {
                auto x_center = rand() % MapArea::GetMapSize().width;
                auto y_center = rand() % MapArea::GetMapSize().height;
                auto r = 3 + rand() % 10;
                auto red_variation = ((float)rand()) / RAND_MAX * 0.2f;
                auto green_variation = ((float)rand()) / RAND_MAX * 0.4f;
                auto blue_variation = ((float)rand()) / RAND_MAX * 0.2f;
                for (auto y = y_center - r; y <= y_center + r; y++)
                /***********************************************/
                {
                    for (auto x = x_center - r; x <= x_center + r; x++)
                    /***********************************************/
                    {
                        if (x < 0 || y < 0 || x >= MapArea::GetMapSize().width ||
                            y >= MapArea::GetMapSize().height)
                            continue;
                        if (map_area->GetTile({x, y})->GetGroundType() == Hash("GroundWater"))
                            continue;
                        auto dx = x - x_center;
                        auto dy = y - y_center;
                        if (dx * dx + dy * dy < r * r)
                        /****************************/
                        {
                            if (map_area->GetTile({x, y})->GetGroundType() != Hash("GroundRock"))
                            /******************************************************************/
                            {
                                map_area->GetTile({x, y})->SetRedVariation(red_variation);
                                map_area->GetTile({x, y})->SetGreenVariation(green_variation);
                                map_area->GetTile({x, y})->SetBlueVariation(blue_variation);
                            }
                            else
                            /**/
                            {
                                map_area->GetTile({x, y})->SetRedVariation(red_variation / 3.0f);
                                map_area->GetTile({x, y})->SetGreenVariation(green_variation / 3.0f);
                                map_area->GetTile({x, y})->SetBlueVariation(blue_variation / 3.0f);
                            }
                            for (auto &object : map_area->GetTile(x, y)->GetObjects().list_)
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