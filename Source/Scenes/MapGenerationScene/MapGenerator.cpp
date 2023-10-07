#include "MapGenerator.hpp"
#include "Engine/Core/TextOutBox.hpp"
#include "World/Actors/Mob.hpp"
#include "World/MapArea.hpp"
#include "World/Object.hpp"
#include "World/ObjectsCollection.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void
    WorldMapGenerator::DoGenerationStep(GenerationSteps generationStep)
    /*////////////////////////////////////////////////////////////*/
    {
        switch (generationStep)
        /*********************/
        {
        case GenerationSteps::CreateBlank:
            /***************************/
            {
                TextOutBox::Get()->Print("Creating blank map.");
                new_map_area_ = std::make_shared<MapArea>();
                new_map_area_->Create();
                World::Get()->AddMapAreaAtZLevel(0, new_map_area_);
                break;
            }
        case GenerationSteps::GenWater:
            /************************/
            {
                TextOutBox::Get()->Print("Generating water.");
                AddWater();
                break;
            }
        case GenerationSteps::GenElev:
            /***********************/
            {
                TextOutBox::Get()->Print("Generating elevation.");
                AddElevation();
                break;
            }
        case GenerationSteps::GenRock:
            /***********************/
            {
                TextOutBox::Get()->Print("Generating rock.");
                AddWaterRivers();
                break;
            }
        case GenerationSteps::GenObjects:
            /**************************/
            {
                TextOutBox::Get()->Print("Generating objects.");
                AddObjects();
                break;
            }
        case GenerationSteps::GenMobs:
            /***********************/
            {
                TextOutBox::Get()->Print("Generating animals.");
                AddMobs();
                break;
            }
        case GenerationSteps::GenNpcs:
            /***********************/
            { break; }
        case GenerationSteps::Finalize:
            /************************/
            {
                GenerateColorVariations();
                TextOutBox::Get()->Print("Finalizing map.");
                TextOutBox::Get()->Print("Map terrain generation finished.");
                auto underground_map = std::make_shared<MapArea>();
                underground_map->Create();
                for (auto y = 0; y < MapArea::GetMapSize().height; y++)
                /*****************************************************/
                {
                    for (auto x = 0; x < MapArea::GetMapSize().width; x++)
                    /****************************************************/
                    {
                        underground_map->GetTile({x, y})->SetGroundType(Hash("GroundCaveFloor"));
                        underground_map->GetTile({x, y})->CreateAddObject("ObjectCaveBlock");
                        World::Get()->AddMapAreaAtZLevel(-1, underground_map);
                    }
                }
                break;
            }
        }
    }

    void
    WorldMapGenerator::AddElevation()
    /*////////////////////////////*/
    {
        auto map_area = World::Get()->GetMapAreaAtZLevel(0);
        auto area_count = 8;
        for (auto i = 0; i < area_count; i++)
        /**********************************/
        {
            auto num_chained_mountains = rand() % 3;
            auto x_center = rand() % MapArea::GetMapSize().width;
            auto y_center = rand() % MapArea::GetMapSize().height;
            auto width = 12 + rand() % 57;
            auto depth = 12 + rand() % 57;
            auto max_dim = std::max(width, depth);
            auto size = 5 + rand() % 5;
            float elev_value = -1.0f;
            bool hill = (bool)(rand() % 2);
            bool rock = false;
            for (auto j = 0; j < num_chained_mountains; j++)
            /********************************************/
            {
                for (auto r = max_dim; r > 3; r--)
                /*******************************/
                {
                    for (auto y = y_center - r; y <= y_center + r; y++)
                    /***********************************************/
                    {
                        for (auto x = x_center - r; x <= x_center + r; x++)
                        /***********************************************/
                        {
                            if (!MapArea::IsInsideMap({x, y}))
                                continue;
                            auto tile = map_area->GetTile({x, y});
                            if (rand() % 4 == 0 && tile->GetElevation() > 15 && !hill)
                                rock = true;
                            auto dx = x - x_center;
                            auto dy = y - y_center;
                            if (elev_value == -1.0f ||
                                (rand() % 2 == 0 && x == x_center - r && y == y_center - r))
                            /************************************************************/
                            {
                                auto elev_add_tile = 10.5f * static_cast<float>(rand()) / RAND_MAX;
                                elev_value = tile->GetElevation() + elev_add_tile;
                            }
                            if (dx * dx * width + dy * dy * depth <= width * depth * size)
                            /************************************************************/
                            {
                                auto tile_is_water = tile->GetGroundType() == Hash("GroundWater");
                                auto west_tile_is_water = false;
                                auto north_tile_is_water = false;
                                auto north_west_tile_is_water = false;
                                if (MapArea::IsInsideMap({x - 1, y}))
                                /***********************************/
                                {
                                    west_tile_is_water =
                                        map_area->GetTile({x - 1, y})->GetGroundType() ==
                                        Hash("GroundWater");
                                }
                                if (MapArea::IsInsideMap({x, y - 1}))
                                /***********************************/
                                {
                                    north_tile_is_water =
                                        map_area->GetTile({x, y - 1})->GetGroundType() ==
                                        Hash("GroundWater");
                                }
                                if (MapArea::IsInsideMap({x - 1, y - 1}))
                                /***************************************/
                                {
                                    north_west_tile_is_water =
                                        map_area->GetTile({x - 1, y - 1})->GetGroundType() ==
                                        Hash("GroundWater");
                                }
                                if (!tile_is_water && !west_tile_is_water && !north_tile_is_water &&
                                    !north_west_tile_is_water)
                                /**********************************************************/
                                {
                                    tile->SetElevation((tile->GetElevation() + elev_value) / 2);
                                    if (rock)
                                        tile->SetGroundType(Hash("GroundRock"));
                                }
                            }
                        }
                    }
                }
            }
            x_center += rand() % 5 - rand() % 5;
            y_center += rand() % 5 - rand() % 5;
        }
        for (auto y = 0; y < MapArea::GetMapSize().height; y++)
        /*****************************************************/
        {
            for (auto x = 0; x < MapArea::GetMapSize().width; x++)
            /****************************************************/
            {
                auto tile = map_area->GetTile({x, y});
                auto tile_is_water = tile->GetGroundType() == Hash("GroundWater");
                auto west_tile_is_water = false;
                auto north_tile_is_water = false;
                auto north_west_tile_is_water = false;
                if (MapArea::IsInsideMap({x - 1, y}))
                /***********************************/
                {
                    west_tile_is_water =
                        map_area->GetTile({x - 1, y})->GetGroundType() == Hash("GroundWater");
                }
                if (MapArea::IsInsideMap({x, y - 1}))
                /***********************************/
                {
                    north_tile_is_water =
                        map_area->GetTile({x, y - 1})->GetGroundType() == Hash("GroundWater");
                }
                if (MapArea::IsInsideMap({x - 1, y - 1}))
                /***************************************/
                {
                    north_west_tile_is_water =
                        map_area->GetTile({x - 1, y - 1})->GetGroundType() == Hash("GroundWater");
                }
                if (!tile_is_water && !west_tile_is_water && !north_tile_is_water && !north_west_tile_is_water)
                /*********************************************************************************/
                {
                    auto elev_add_tile = 1.0f * static_cast<float>(rand()) / RAND_MAX;
                    tile->AlterElevation(elev_add_tile);
                    tile->AlterElevation(2.0f);
                }
            }
        }
    }

    void
    WorldMapGenerator::AddMobs()
    /*///////////////////////*/
    {
        auto map_area = World::Get()->GetMapAreaAtZLevel(0);
        auto num_boars = 60;
        for (auto i = 0; i < num_boars; i++)
        /*********************************/
        {
            auto x = rand() % MapArea::GetMapSize().width;
            auto y = rand() % MapArea::GetMapSize().height;
            auto tile = map_area->GetTile({x, y});
            if (tile->GetGroundType() != Hash("GroundWater"))
            /***********************************************/
            {
                if (tile->GetMob() == nullptr)
                /****************************/
                {
                    tile->SetMob(std::make_shared<Mob>(Hash("MobTypeBoar"), x, y));
                    map_area->AddMobMirror(tile->GetMob().get(), {x, y});
                }
            }
        }
        auto num_birds = 50;
        for (auto i = 0; i < num_birds; i++)
        /*********************************/
        {
            auto x = rand() % MapArea::GetMapSize().width;
            auto y = rand() % MapArea::GetMapSize().height;
            auto tile = map_area->GetTile({x, y});
            if (tile->GetMob() == nullptr)
            /****************************/
            {
                tile->SetMob(std::make_shared<Mob>(Hash("MobTypeBird1"), x, y));
                tile->GetMob()->SetDistanceAboveGround(8.0f);
                tile->GetMob()->SetMoveSpeed(300);
                map_area->AddMobMirror(tile->GetMob().get(), {x, y});
            }
        }
    }

    void
    WorldMapGenerator::AddObjects()
    /*//////////////////////////*/
    {
        auto map_area = World::Get()->GetMapAreaAtZLevel(0);
        auto forests_count = 70;
        for (auto i = 0; i < forests_count; i++)
        /*************************************/
        {
            auto x = rand() % MapArea::GetMapSize().width;
            auto y = rand() % MapArea::GetMapSize().height;
            auto num_trees_in_forest = 14 + rand() % 32;
            for (auto j = 0; j < num_trees_in_forest; j++)
            /*****************************************/
            {
                if (MapArea::IsInsideMap({x, y}))
                /*******************************/
                {
                    auto tree_type = rand() % 2;
                    auto tile = map_area->GetTile({x, y});
                    if (tile->GetGroundType() == Hash("GroundGrass"))
                    /***********************************************/
                    {
                        if (tile->GetObjects().list.size() < 2)
                        /*************************************/
                        {
                            if (tree_type == 0)
                            /****************/
                            {
                                auto new_object = std::make_shared<Object>("ObjectTree1");
                                new_object->SetModelScaling(
                                    0.7f + static_cast<float>(rand()) / RAND_MAX * 0.6f);
                                new_object->SetModelRotation(
                                    static_cast<float>(rand()) / RAND_MAX * 360.0f);
                                tile->AddObject(new_object);
                            }
                            else
                            /**/
                            {
                                auto new_object = std::make_shared<Object>("ObjectTree2");
                                new_object->SetModelScaling(
                                    0.6f + static_cast<float>(rand()) / RAND_MAX * 0.2f);
                                new_object->SetModelRotation(
                                    static_cast<float>(rand()) / RAND_MAX * 360.0f);
                                tile->AddObject(new_object);
                            }
                        }
                    }
                    x += rand() % 4 - rand() % 4;
                    y += rand() % 4 - rand() % 4;
                }
            }
        }
        AddObjects("ObjectStoneBoulder", 150, "GroundGrass");
        AddObjects("ObjectStoneBoulder", 150, "GroundRock");
        AddObjects("ObjectBush1", 300, "GroundGrass");
        AddObjects("ObjectPinkFlower", 100, "GroundGrass");
        AddObjects("ObjectChampignon", 100, "GroundGrass");
        AddObjects("ObjectSmallStone", 150, "GroundGrass");
        AddObjects("ObjectSmallStone", 150, "GroundRock");
        AddObjects("ObjectBranch", 300, "GroundGrass");
        for (auto y = 0; y < MapArea::GetMapSize().height; y++)
        /*****************************************************/
        {
            for (auto x = 0; x < MapArea::GetMapSize().width; x++)
            /****************************************************/
            {
                auto tile = map_area->GetTile({x, y});
                if (tile->GetGroundType() == Hash("GroundGrass"))
                /***********************************************/
                {
                    if (false == tile->GetObjects().Contains(Hash("ObjectTallGrass6")))
                    /*****************************************************************/
                    {
                        auto new_object = std::make_shared<Object>("ObjectTallGrass6");
                        new_object->SetModelScaling(
                            0.1f + static_cast<float>(rand()) / RAND_MAX * 0.3f);
                        new_object->SetModelRotation(static_cast<float>(rand()) / RAND_MAX * 360.0f);
                        tile->AddObject(new_object);
                    }
                }
            }
        }
    }

    void
    WorldMapGenerator::AddWater()
    /*////////////////////////*/
    {
        auto map_area = World::Get()->GetMapAreaAtZLevel(0);
        auto area_count = 30;
        for (auto i = 0; i < area_count; i++)
        /**********************************/
        {
            auto generate_new_center = true;
            int x_center;
            int y_center;
            while (generate_new_center)
            /***********************/
            {
                x_center = rand() % MapArea::GetMapSize().width;
                y_center = rand() % MapArea::GetMapSize().height;
                if ((x_center < MapArea::GetMapSize().width * 3 / 8 ||
                     x_center >= MapArea::GetMapSize().width * 5 / 8) &&
                    (y_center < MapArea::GetMapSize().height * 3 / 8 ||
                     y_center >= MapArea::GetMapSize().height * 5 / 8))
                    generate_new_center = false;
            }
            auto dim1 = 1 + (rand() % 3) * 4;
            auto dim2 = 1 + (rand() % 3) * 4;
            auto max_dim = std::max(dim1, dim2);
            for (auto y = y_center - max_dim; y <= y_center + max_dim; y++)
            /*********************************************************/
            {
                for (auto x = x_center - max_dim; x <= x_center + max_dim; x++)
                /*********************************************************/
                {
                    if (!MapArea::IsInsideMap({x, y}))
                        continue;
                    auto dx = x - x_center;
                    auto dy = y - y_center;
                    if (dx * dx * dim1 / max_dim + dy * dy * dim2 / max_dim <= dim1 * dim2)
                    /*******************************************************************/
                    {
                        auto tile = map_area->GetTile({x, y});
                        tile->SetGroundType(Hash("GroundWater"));
                    }
                }
            }
        }
    }

    void
    WorldMapGenerator::AddWaterRivers()
    /*//////////////////////////////*/
    {
        auto map_area = World::Get()->GetMapAreaAtZLevel(0);
        std::vector<Point2F> predefined_spawn_locations = {
            {0.05f, 0.05f}, {0.15f, 0.05f}, {0.25f, 0.05f}, {0.35f, 0.05f}, {0.45f, 0.05f},
            {0.55f, 0.05f}, {0.65f, 0.05f}, {0.75f, 0.05f}, {0.85f, 0.05f}, {0.95f, 0.05f},
            {0.05f, 0.15f}, {0.15f, 0.15f}, {0.25f, 0.15f}, {0.35f, 0.15f}, {0.45f, 0.15f},
            {0.55f, 0.15f}, {0.65f, 0.15f}, {0.75f, 0.15f}, {0.85f, 0.15f}, {0.95f, 0.15f},
            {0.05f, 0.25f}, {0.15f, 0.25f}, {0.25f, 0.25f}, {0.35f, 0.25f}, {0.45f, 0.25f},
            {0.55f, 0.25f}, {0.65f, 0.25f}, {0.75f, 0.25f}, {0.85f, 0.25f}, {0.95f, 0.25f},
            {0.05f, 0.35f}, {0.15f, 0.35f}, {0.25f, 0.35f}, {0.35f, 0.35f}, {0.45f, 0.35f},
            {0.55f, 0.35f}, {0.65f, 0.35f}, {0.75f, 0.35f}, {0.85f, 0.35f}, {0.95f, 0.35f},
            {0.05f, 0.45f}, {0.15f, 0.45f}, {0.25f, 0.45f}, {0.35f, 0.45f}, {0.45f, 0.45f},
            {0.55f, 0.45f}, {0.65f, 0.45f}, {0.75f, 0.45f}, {0.85f, 0.45f}, {0.95f, 0.45f},
            {0.05f, 0.55f}, {0.15f, 0.55f}, {0.25f, 0.55f}, {0.35f, 0.55f}, {0.45f, 0.55f},
            {0.55f, 0.55f}, {0.65f, 0.55f}, {0.75f, 0.55f}, {0.85f, 0.55f}, {0.95f, 0.55f},
            {0.05f, 0.65f}, {0.15f, 0.65f}, {0.25f, 0.65f}, {0.35f, 0.65f}, {0.45f, 0.65f},
            {0.55f, 0.65f}, {0.65f, 0.65f}, {0.75f, 0.65f}, {0.85f, 0.65f}, {0.95f, 0.65f},
            {0.05f, 0.75f}, {0.15f, 0.75f}, {0.25f, 0.75f}, {0.35f, 0.75f}, {0.45f, 0.75f},
            {0.55f, 0.75f}, {0.65f, 0.75f}, {0.75f, 0.75f}, {0.85f, 0.75f}, {0.95f, 0.75f},
            {0.05f, 0.85f}, {0.15f, 0.85f}, {0.25f, 0.85f}, {0.35f, 0.85f}, {0.45f, 0.85f},
            {0.55f, 0.85f}, {0.65f, 0.85f}, {0.75f, 0.85f}, {0.85f, 0.85f}, {0.95f, 0.85f},
            {0.05f, 0.95f}, {0.15f, 0.95f}, {0.25f, 0.95f}, {0.35f, 0.95f}, {0.45f, 0.95f},
            {0.55f, 0.95f}, {0.65f, 0.95f}, {0.75f, 0.95f}, {0.85f, 0.95f}, {0.95f, 0.95f},
        };
        std::vector<Point2F> used_locations;
        auto river_count = 8;
        for (auto i = 0; i < river_count; i++)
        /***********************************/
        {
            Point2F location;
            bool location_already_taken;
            do
            /*************************************************/
            {
                location = predefined_spawn_locations.at((rand() % 25) * 4);
                location_already_taken = false;
                for (auto &entry : used_locations)
                    if (entry.x == location.x && entry.y == location.y)
                        location_already_taken = true;
            } while (location_already_taken);
            used_locations.push_back(location);
            float x_f = location.x * MapArea::GetMapSize().width;
            float y_f = location.y * MapArea::GetMapSize().height;
            auto num_sections = 3 + rand() % 30;
            float dx = (float)rand() / RAND_MAX - (float)rand() / RAND_MAX;
            float dy = (float)rand() / RAND_MAX - (float)rand() / RAND_MAX;
            for (auto k = 0; k < num_sections; k++)
            /************************************/
            {
                auto num_steps = 2 + rand() % 20;
                for (auto j = 0; j < num_steps; j++)
                /*********************************/
                {
                    auto x_i = (int)x_f;
                    auto y_i = (int)y_f;
                    if (MapArea::IsInsideMap({x_i, y_i}))
                    /*********************************/
                    {
                        if (map_area->GetTile({x_i, y_i})->GetGroundType() == Hash("GroundRock"))
                        /********************************************************************/
                        {
                            map_area->GetTile({x_i, y_i})->AlterElevation(-2);
                            map_area->GetTile({x_i, y_i})->SetElevation(
                                std::max(-1.0f, map_area->GetTile({x_i, y_i})->GetElevation()));
                            if (x_i < MapArea::GetMapSize().width - 1)
                            /***************************************/
                            {
                                if (map_area->GetTile({x_i + 1, y_i})->GetGroundType() !=
                                    Hash("GroundWater"))
                                    map_area->GetTile({x_i + 1, y_i})
                                        ->SetElevation(map_area->GetTile({x_i, y_i})->GetElevation());
                            }
                            if (y_i < MapArea::GetMapSize().height - 1)
                            /****************************************/
                            {
                                if (map_area->GetTile({x_i, y_i + 1})->GetGroundType() !=
                                    Hash("GroundWater"))
                                    map_area->GetTile({x_i, y_i + 1})
                                        ->SetElevation(map_area->GetTile({x_i, y_i})->GetElevation());
                            }
                            if (x_i < MapArea::GetMapSize().width - 1 &&
                                y_i < MapArea::GetMapSize().height - 1)
                            /*****************************************/
                            {
                                if (map_area->GetTile({x_i + 1, y_i + 1})->GetGroundType() !=
                                    Hash("GroundWater"))
                                    map_area->GetTile({x_i + 1, y_i + 1})
                                        ->SetElevation(map_area->GetTile({x_i, y_i})->GetElevation());
                            }
                        }
                        if (map_area->GetTile({x_i, y_i})->GetGroundType() == Hash("GroundGrass"))
                        /*********************************************************************/
                        {
                            map_area->GetTile({x_i, y_i})->SetGroundType(Hash("GroundRiver"));
                        }
                    }
                    x_f += dx;
                    y_f += dy;
                }
                auto angle_change = (float)rand() / RAND_MAX * M_PI / 4;
                auto current_angle = std::atan2(dy, dx);
                auto new_angle = current_angle + angle_change;
                auto new_dx = std::cos(new_angle);
                auto new_dy = std::sin(new_angle);
                auto norm_x = 0;
                auto norm_y = 0;
                if (new_dx)
                    norm_x = new_dx / std::abs(new_dx);
                if (new_dy)
                    norm_y = new_dy / std::abs(new_dy);
                auto next_x = (int)(x_f + norm_x);
                auto next_y = (int)(y_f + norm_y);
                auto current_x = (int)x_f;
                auto current_y = (int)y_f;
                if (next_x >= 0 && next_y >= 0 && next_x < MapArea::GetMapSize().width &&
                    next_y < MapArea::GetMapSize().height && current_x >= 0 && current_y >= 0 &&
                    current_x < MapArea::GetMapSize().width && current_y < MapArea::GetMapSize().height)
                /******************************************************************************/
                {
                    if (map_area->GetTile({next_x, next_y})->GetElevation() >
                        map_area->GetTile({current_x, current_y})->GetElevation())
                    /****************************************************/
                    {
                        continue;
                    }
                }
                dx = new_dx;
                dy = new_dy;
            }
        }
    }

    void
    WorldMapGenerator::AddObjects(
        std::string_view objectName, int amount, std::string_view groundType)
    /*/////////////////////////////////////////////////////////////////////*/
    {
        auto map_area = World::Get()->GetMapAreaAtZLevel(0);
        for (auto i = 0; i < amount; i++)
        /*******************************/
        {
            auto x = rand() % MapArea::GetMapSize().width;
            auto y = rand() % MapArea::GetMapSize().height;
            auto tile = map_area->GetTile({x, y});
            if (tile->GetGroundType() == Hash(groundType))
            /********************************************/
            {
                auto new_object = std::make_shared<Object>(objectName);
                new_object->SetModelScaling(0.8f + static_cast<float>(rand()) / RAND_MAX * 0.5f);
                new_object->SetModelRotation(static_cast<float>(rand()) / RAND_MAX * 360.0f);
                tile->AddObject(new_object);
            }
        }
    }

    void
    WorldMapGenerator::GenerateColorVariations()
    /*///////////////////////////////////////*/
    {
        World::Get()->ResetColorVariations();
        World::Get()->GenerateRandomColorVariations();
        World::Get()->ApplyDefaultColorVariations();
    }
}