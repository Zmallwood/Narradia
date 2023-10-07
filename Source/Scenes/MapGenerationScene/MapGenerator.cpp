//////////////////////////////////////////////////////////////////////
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
    WorldMapGenerate::DoGenerationStep(GenerateSteps generationStep)
    /*////////////////////////////////////////////////////////////*/
    {
        switch (generationStep)
        /*********************/
        {
        case GenerateSteps::CreateBlank:
            /***************************/
            {
                TextOutBox::Get()->Print("Creating blank map.");
                newMapArea = std::make_shared<MapArea>();
                newMapArea->Create();
                World::Get()->AddMapAreaAtZLevel(0, newMapArea);
                break;
            }
        case GenerateSteps::GenWater:
            /************************/
            {
                TextOutBox::Get()->Print("Generating water.");
                AddWater();
                break;
            }
        case GenerateSteps::GenElev:
            /***********************/
            {
                TextOutBox::Get()->Print("Generating elevation.");
                AddElevation();
                break;
            }
        case GenerateSteps::GenRock:
            /***********************/
            {
                TextOutBox::Get()->Print("Generating rock.");
                AddWaterRivers();
                break;
            }
        case GenerateSteps::GenObjects:
            /**************************/
            {
                TextOutBox::Get()->Print("Generating objects.");
                AddObjects();
                break;
            }
        case GenerateSteps::GenMobs:
            /***********************/
            {
                TextOutBox::Get()->Print("Generating animals.");
                AddMobs();
                break;
            }
        case GenerateSteps::GenNpcs:
            /***********************/
            { break; }
        case GenerateSteps::Finalize:
            /************************/
            {
                GenerateColorVariations();
                TextOutBox::Get()->Print("Finalizing map.");
                TextOutBox::Get()->Print("Map terrain generation finished.");
                auto undergroundMap = std::make_shared<MapArea>();
                undergroundMap->Create();
                for (auto y = 0; y < MapArea::GetMapSize().height; y++)
                /*****************************************************/
                {
                    for (auto x = 0; x < MapArea::GetMapSize().width; x++)
                    /****************************************************/
                    {
                        undergroundMap->GetTile({x, y})->SetGroundType(Hash("GroundCaveFloor"));
                        undergroundMap->GetTile({x, y})->CreateAddObject("ObjectCaveBlock");
                        World::Get()->AddMapAreaAtZLevel(-1, undergroundMap);
                    }
                }
                break;
            }
        }
    }

    void
    WorldMapGenerate::AddElevation()
    /*////////////////////////////*/
    {
        auto mapArea = World::Get()->GetMapAreaAtZLevel(0);
        auto areaCount = 8;
        for (auto i = 0; i < areaCount; i++)
        /**********************************/
        {
            auto numChainedMountains = rand() % 3;
            auto xCenter = rand() % MapArea::GetMapSize().width;
            auto yCenter = rand() % MapArea::GetMapSize().height;
            auto width = 12 + rand() % 57;
            auto depth = 12 + rand() % 57;
            auto maxDim = std::max(width, depth);
            auto size = 5 + rand() % 5;
            auto elevAddBase = 1.0f * (static_cast<float>(rand()) / RAND_MAX * 1.5f);
            float elevValue = -1.0f;
            bool hill = (bool)(rand() % 2);
            bool rock = false;
            for (auto j = 0; j < numChainedMountains; j++)
            /********************************************/
            {
                for (auto r = maxDim; r > 3; r--)
                /*******************************/
                {
                    for (auto y = yCenter - r; y <= yCenter + r; y++)
                    /***********************************************/
                    {
                        for (auto x = xCenter - r; x <= xCenter + r; x++)
                        /***********************************************/
                        {
                            if (!MapArea::IsInsideMap({x, y}))
                                continue;
                            auto tile = mapArea->GetTile({x, y});
                            if (rand() % 4 == 0 && tile->GetElevation() > 15 && !hill)
                                rock = true;
                            auto dx = x - xCenter;
                            auto dy = y - yCenter;
                            if (elevValue == -1.0f ||
                                (rand() % 2 == 0 && x == xCenter - r && y == yCenter - r))
                            /************************************************************/
                            {
                                auto localRadius = std::sqrt(dx * dx + dy * dy);
                                auto elevAddTile = 10.5f * static_cast<float>(rand()) / RAND_MAX;
                                elevValue = tile->GetElevation() + elevAddTile;
                            }
                            if (dx * dx * width + dy * dy * depth <= width * depth * size)
                            /************************************************************/
                            {
                                auto tileIsWater = tile->GetGroundType() == Hash("GroundWater");
                                auto westTileIsWater = false;
                                auto northTileIsWater = false;
                                auto northwestTileIsWater = false;
                                if (MapArea::IsInsideMap({x - 1, y}))
                                /***********************************/
                                {
                                    westTileIsWater =
                                        mapArea->GetTile({x - 1, y})->GetGroundType() ==
                                        Hash("GroundWater");
                                }
                                if (MapArea::IsInsideMap({x, y - 1}))
                                /***********************************/
                                {
                                    northTileIsWater =
                                        mapArea->GetTile({x, y - 1})->GetGroundType() ==
                                        Hash("GroundWater");
                                }
                                if (MapArea::IsInsideMap({x - 1, y - 1}))
                                /***************************************/
                                {
                                    northwestTileIsWater =
                                        mapArea->GetTile({x - 1, y - 1})->GetGroundType() ==
                                        Hash("GroundWater");
                                }
                                if (!tileIsWater && !westTileIsWater && !northTileIsWater &&
                                    !northwestTileIsWater)
                                /**********************************************************/
                                {
                                    tile->SetElevation((tile->GetElevation() + elevValue) / 2);
                                    if (rock)
                                        tile->SetGroundType(Hash("GroundRock"));
                                }
                            }
                        }
                    }
                }
            }
            xCenter += rand() % 5 - rand() % 5;
            yCenter += rand() % 5 - rand() % 5;
        }
        for (auto y = 0; y < MapArea::GetMapSize().height; y++)
        /*****************************************************/
        {
            for (auto x = 0; x < MapArea::GetMapSize().width; x++)
            /****************************************************/
            {
                auto tile = mapArea->GetTile({x, y});
                auto tileIsWater = tile->GetGroundType() == Hash("GroundWater");
                auto westTileIsWater = false;
                auto northTileIsWater = false;
                auto northwestTileIsWater = false;
                if (MapArea::IsInsideMap({x - 1, y}))
                /***********************************/
                {
                    westTileIsWater =
                        mapArea->GetTile({x - 1, y})->GetGroundType() == Hash("GroundWater");
                }
                if (MapArea::IsInsideMap({x, y - 1}))
                /***********************************/
                {
                    northTileIsWater =
                        mapArea->GetTile({x, y - 1})->GetGroundType() == Hash("GroundWater");
                }
                if (MapArea::IsInsideMap({x - 1, y - 1}))
                /***************************************/
                {
                    northwestTileIsWater =
                        mapArea->GetTile({x - 1, y - 1})->GetGroundType() == Hash("GroundWater");
                }
                if (!tileIsWater && !westTileIsWater && !northTileIsWater && !northwestTileIsWater)
                /*********************************************************************************/
                {
                    auto elevAddTile = 1.0f * static_cast<float>(rand()) / RAND_MAX;
                    tile->AlterElevation(elevAddTile);
                    tile->AlterElevation(2.0f);
                }
            }
        }
    }

    void
    WorldMapGenerate::AddMobs()
    /*///////////////////////*/
    {
        auto mapArea = World::Get()->GetMapAreaAtZLevel(0);
        auto numBoars = 60;
        for (auto i = 0; i < numBoars; i++)
        /*********************************/
        {
            auto x = rand() % MapArea::GetMapSize().width;
            auto y = rand() % MapArea::GetMapSize().height;
            auto tile = mapArea->GetTile({x, y});
            if (tile->GetGroundType() != Hash("GroundWater"))
            /***********************************************/
            {
                if (tile->GetMob() == nullptr)
                /****************************/
                {
                    tile->SetMob(std::make_shared<Mob>(Hash("MobTypeBoar"), x, y));
                    mapArea->AddMobMirror(tile->GetMob().get(), {x, y});
                }
            }
        }
        auto numBirds = 50;
        for (auto i = 0; i < numBirds; i++)
        /*********************************/
        {
            auto x = rand() % MapArea::GetMapSize().width;
            auto y = rand() % MapArea::GetMapSize().height;
            auto tile = mapArea->GetTile({x, y});
            if (tile->GetMob() == nullptr)
            /****************************/
            {
                tile->SetMob(std::make_shared<Mob>(Hash("MobTypeBird1"), x, y));
                tile->GetMob()->SetDistanceAboveGround(8.0f);
                tile->GetMob()->SetMoveSpeed(300);
                mapArea->AddMobMirror(tile->GetMob().get(), {x, y});
            }
        }
    }

    void
    WorldMapGenerate::AddObjects()
    /*//////////////////////////*/
    {
        auto mapArea = World::Get()->GetMapAreaAtZLevel(0);
        auto forestsCount = 70;
        for (auto i = 0; i < forestsCount; i++)
        /*************************************/
        {
            auto x = rand() % MapArea::GetMapSize().width;
            auto y = rand() % MapArea::GetMapSize().height;
            auto numTreesInForest = 14 + rand() % 32;
            for (auto j = 0; j < numTreesInForest; j++)
            /*****************************************/
            {
                if (MapArea::IsInsideMap({x, y}))
                /*******************************/
                {
                    auto treeType = rand() % 2;
                    auto tile = mapArea->GetTile({x, y});
                    if (tile->GetGroundType() == Hash("GroundGrass"))
                    /***********************************************/
                    {
                        if (tile->GetObjects().list.size() < 2)
                        /*************************************/
                        {
                            if (treeType == 0)
                            /****************/
                            {
                                auto newObject = std::make_shared<Object>("ObjectTree1");
                                newObject->SetModelScaling(
                                    0.7f + static_cast<float>(rand()) / RAND_MAX * 0.6f);
                                newObject->SetModelRotation(
                                    static_cast<float>(rand()) / RAND_MAX * 360.0f);
                                tile->AddObject(newObject);
                            }
                            else
                            /**/
                            {
                                auto newObject = std::make_shared<Object>("ObjectTree2");
                                newObject->SetModelScaling(
                                    0.6f + static_cast<float>(rand()) / RAND_MAX * 0.2f);
                                newObject->SetModelRotation(
                                    static_cast<float>(rand()) / RAND_MAX * 360.0f);
                                tile->AddObject(newObject);
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
        auto tallGrassCount = 10000;
        for (auto y = 0; y < MapArea::GetMapSize().height; y++)
        /*****************************************************/
        {
            for (auto x = 0; x < MapArea::GetMapSize().width; x++)
            /****************************************************/
            {
                auto tile = mapArea->GetTile({x, y});
                if (tile->GetGroundType() == Hash("GroundGrass"))
                /***********************************************/
                {
                    if (false == tile->GetObjects().Contains(Hash("ObjectTallGrass6")))
                    /*****************************************************************/
                    {
                        auto newObject = std::make_shared<Object>("ObjectTallGrass6");
                        newObject->SetModelScaling(
                            0.1f + static_cast<float>(rand()) / RAND_MAX * 0.3f);
                        newObject->SetModelRotation(static_cast<float>(rand()) / RAND_MAX * 360.0f);
                        tile->AddObject(newObject);
                    }
                }
            }
        }
    }

    void
    WorldMapGenerate::AddWater()
    /*////////////////////////*/
    {
        auto mapArea = World::Get()->GetMapAreaAtZLevel(0);
        auto areaCount = 30;
        for (auto i = 0; i < areaCount; i++)
        /**********************************/
        {
            auto generateNewCenter = true;
            int xCenter;
            int yCenter;
            while (generateNewCenter)
            /***********************/
            {
                xCenter = rand() % MapArea::GetMapSize().width;
                yCenter = rand() % MapArea::GetMapSize().height;
                if ((xCenter < MapArea::GetMapSize().width * 3 / 8 ||
                     xCenter >= MapArea::GetMapSize().width * 5 / 8) &&
                    (yCenter < MapArea::GetMapSize().height * 3 / 8 ||
                     yCenter >= MapArea::GetMapSize().height * 5 / 8))
                    generateNewCenter = false;
            }
            auto dim1 = 1 + (rand() % 3) * 4;
            auto dim2 = 1 + (rand() % 3) * 4;
            auto maxDim = std::max(dim1, dim2);
            for (auto y = yCenter - maxDim; y <= yCenter + maxDim; y++)
            /*********************************************************/
            {
                for (auto x = xCenter - maxDim; x <= xCenter + maxDim; x++)
                /*********************************************************/
                {
                    if (!MapArea::IsInsideMap({x, y}))
                        continue;
                    auto dx = x - xCenter;
                    auto dy = y - yCenter;
                    if (dx * dx * dim1 / maxDim + dy * dy * dim2 / maxDim <= dim1 * dim2)
                    /*******************************************************************/
                    {
                        auto tile = mapArea->GetTile({x, y});
                        tile->SetGroundType(Hash("GroundWater"));
                    }
                }
            }
        }
    }

    void
    WorldMapGenerate::AddWaterRivers()
    /*//////////////////////////////*/
    {
        auto mapArea = World::Get()->GetMapAreaAtZLevel(0);
        std::vector<Point2F> predefinedSpawnLocs = {
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
        std::vector<Point2F> usedLocs;
        auto riverCount = 8;
        for (auto i = 0; i < riverCount; i++)
        /***********************************/
        {
            Point2F loc;
            bool locAlreadyTaken;
            do
            /*************************************************/
            {
                loc = predefinedSpawnLocs.at((rand() % 25) * 4);
                locAlreadyTaken = false;
                for (auto &entry : usedLocs)
                    if (entry.x == loc.x && entry.y == loc.y)
                        locAlreadyTaken = true;
            } while (locAlreadyTaken);
            usedLocs.push_back(loc);
            float xF = loc.x * MapArea::GetMapSize().width;
            float yF = loc.y * MapArea::GetMapSize().height;
            auto numSections = 3 + rand() % 30;
            float dx = (float)rand() / RAND_MAX - (float)rand() / RAND_MAX;
            float dy = (float)rand() / RAND_MAX - (float)rand() / RAND_MAX;
            for (auto k = 0; k < numSections; k++)
            /************************************/
            {
                auto numSteps = 2 + rand() % 20;
                for (auto j = 0; j < numSteps; j++)
                /*********************************/
                {
                    auto xI = (int)xF;
                    auto yI = (int)yF;
                    if (MapArea::IsInsideMap({xI, yI}))
                    /*********************************/
                    {
                        if (mapArea->GetTile({xI, yI})->GetGroundType() == Hash("GroundRock"))
                        /********************************************************************/
                        {
                            mapArea->GetTile({xI, yI})->AlterElevation(-2);
                            mapArea->GetTile({xI, yI})->SetElevation(
                                std::max(-1.0f, mapArea->GetTile({xI, yI})->GetElevation()));
                            if (xI < MapArea::GetMapSize().width - 1)
                            /***************************************/
                            {
                                if (mapArea->GetTile({xI + 1, yI})->GetGroundType() !=
                                    Hash("GroundWater"))
                                    mapArea->GetTile({xI + 1, yI})
                                        ->SetElevation(mapArea->GetTile({xI, yI})->GetElevation());
                            }
                            if (yI < MapArea::GetMapSize().height - 1)
                            /****************************************/
                            {
                                if (mapArea->GetTile({xI, yI + 1})->GetGroundType() !=
                                    Hash("GroundWater"))
                                    mapArea->GetTile({xI, yI + 1})
                                        ->SetElevation(mapArea->GetTile({xI, yI})->GetElevation());
                            }
                            if (xI < MapArea::GetMapSize().width - 1 &&
                                yI < MapArea::GetMapSize().height - 1)
                            /*****************************************/
                            {
                                if (mapArea->GetTile({xI + 1, yI + 1})->GetGroundType() !=
                                    Hash("GroundWater"))
                                    mapArea->GetTile({xI + 1, yI + 1})
                                        ->SetElevation(mapArea->GetTile({xI, yI})->GetElevation());
                            }
                        }
                        if (mapArea->GetTile({xI, yI})->GetGroundType() == Hash("GroundGrass"))
                        /*********************************************************************/
                        {
                            mapArea->GetTile({xI, yI})->SetGroundType(Hash("GroundRiver"));
                        }
                    }
                    xF += dx;
                    yF += dy;
                }
                auto angleChange = (float)rand() / RAND_MAX * M_PI / 4;
                auto currAngle = std::atan2(dy, dx);
                auto newAngle = currAngle + angleChange;
                auto newDx = std::cos(newAngle);
                auto newDy = std::sin(newAngle);
                auto normX = 0;
                auto normY = 0;
                if (newDx)
                    normX = newDx / std::abs(newDx);
                if (newDy)
                    normX = newDy / std::abs(newDy);
                auto nextX = (int)(xF + normX);
                auto nextY = (int)(yF + normY);
                auto currX = (int)xF;
                auto currY = (int)yF;
                if (nextX >= 0 && nextY >= 0 && nextX < MapArea::GetMapSize().width &&
                    nextY < MapArea::GetMapSize().height && currX >= 0 && currY >= 0 &&
                    currX < MapArea::GetMapSize().width && currY < MapArea::GetMapSize().height)
                /******************************************************************************/
                {
                    if (mapArea->GetTile({nextX, nextY})->GetElevation() >
                        mapArea->GetTile({currX, currY})->GetElevation())
                    /****************************************************/
                    {
                        continue;
                    }
                }
                dx = newDx;
                dy = newDy;
            }
        }
    }

    void
    WorldMapGenerate::AddObjects(
        std::string_view objectName, int amount, std::string_view groundType)
    /*/////////////////////////////////////////////////////////////////////*/
    {
        auto mapArea = World::Get()->GetMapAreaAtZLevel(0);
        for (auto i = 0; i < amount; i++)
        /*******************************/
        {
            auto x = rand() % MapArea::GetMapSize().width;
            auto y = rand() % MapArea::GetMapSize().height;
            auto tile = mapArea->GetTile({x, y});
            if (tile->GetGroundType() == Hash(groundType))
            /********************************************/
            {
                auto newObject = std::make_shared<Object>(objectName);
                newObject->SetModelScaling(0.8f + static_cast<float>(rand()) / RAND_MAX * 0.5f);
                newObject->SetModelRotation(static_cast<float>(rand()) / RAND_MAX * 360.0f);
                tile->AddObject(newObject);
            }
        }
    }

    void
    WorldMapGenerate::GenerateColorVariations()
    /*///////////////////////////////////////*/
    {
        World::Get()->ResetColorVariations();
        World::Get()->GenerateRandomColorVariations();
        World::Get()->ApplyDefaultColorVariations();
    }
}