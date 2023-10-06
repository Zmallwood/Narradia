//////////////////////////////////////////////////////////////////////
#include "MapFilesIO.hpp"
#include "World/MapArea.hpp"
#include "World/Mob.hpp"
#include "World/Object.hpp"
#include "World/ObjectsCollection.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MapFilesIO::Pimpl
    /*///////////////////*/
    {
      public:
        static constexpr std::string_view relMapsPath = "Maps";
    };

    MapFilesIO::MapFilesIO()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void MapFilesIO::SaveCurrentMapArea(std::string_view mapName)
    /*/////////////////////////////////////////////////////////*/
    {
        std::filesystem::create_directory(
            std::string(SDL_GetBasePath()) + Pimpl::relMapsPath.data());
        std::filesystem::remove(
            std::string(SDL_GetBasePath()) + Pimpl::relMapsPath.data() + "/" + mapName.data() +
            ".map");
        std::ofstream mapFile;
        mapFile.open(
            std::string(SDL_GetBasePath()) + Pimpl::relMapsPath.data() + "/" + mapName.data() +
                ".map",
            std::ofstream::out | std::ofstream::trunc);
        auto mapArea = World::Get().GetCurrentMapArea();
        for (auto y = 0; y < MapArea::GetMapSize().height; y++)
        /*****************************************************/
        {
            for (auto x = 0; x < MapArea::GetMapSize().width; x++)
            /****************************************************/
            {
                auto tile = mapArea->GetTile(x, y);
                mapFile << "<tile>\n";
                mapFile << "   <ground>" << tile->GetGroundType() << "</ground>\n";
                mapFile << "   <elevation>" << tile->GetElevation() << "</elevation>\n";
                mapFile << "   <roof>" << tile->GetRoofType() << "</roof>\n";
                mapFile << "   <mob>\n";
                if (tile->GetMob())
                /*****************/
                {
                    mapFile << "<type>" << tile->GetMob()->GetMobType() << "</type>\n";
                    mapFile << "<spawnX>" << tile->GetMob()->GetSpawnX() << "</spawnX>\n";
                    mapFile << "<spawnY>" << tile->GetMob()->GetSpawnY() << "</spawnY>\n";
                }
                else
                /**/
                {
                    mapFile << "<type>" << 0 << "</type>";
                }
                mapFile << "   </mob>\n";
                mapFile << "   <redVariation>" << tile->GetRedVariation() << "</redVariation>\n";
                mapFile << "   <greenVariation>" << tile->GetGreenVariation()
                        << "</greenVariation>\n";
                mapFile << "   <blueVariation>" << tile->GetBlueVariation() << "</blueVariation>\n";
                mapFile << "   <objects>\n";
                for (auto &entry : tile->GetObjects().list)
                /*****************************************/
                {
                    mapFile << "      <object>\n";
                    mapFile << "         <type>" << entry->GetObjectType() << "</type>\n";
                    mapFile << "         <quantity>" << entry->GetQuantity() << "</quantity>\n";
                    mapFile << "      </object>\n";
                }
                mapFile << "   </objects>\n";
                mapFile << "</tile>\n";
            }
        }
        mapFile.close();
        TextOutBox::Get().Print("World saved with name " + std::string(mapName.data()) + ".");
    }

    void MapFilesIO::LoadMapArea(std::string_view mapName)
    /*//////////////////////////////////////////////////*/
    {
        std::ifstream mapFile;
        mapFile.open(
            std::string(SDL_GetBasePath()) + Pimpl::relMapsPath.data() + "/" + mapName.data() +
            ".map");
        World::Get().RemoveMapAreaAtZLevel(0);
        auto newMapArea = std::make_shared<MapArea>();
        newMapArea->Create();
        for (auto y = 0; y < MapArea::GetMapSize().height; y++)
        /*****************************************************/
        {
            for (auto x = 0; x < MapArea::GetMapSize().width; x++)
            /****************************************************/
            {
                auto tile = newMapArea->GetTile(x, y);
                std::string s;
                mapFile >> s;
                while (s != "</tile>")
                /********************/
                {
                    if (s.find("<ground>") != std::string::npos)
                    /******************************************/
                    {
                        auto groundType = std::stoi(
                            s.substr(s.find(">") + 1, s.find_last_of("<") - s.find(">") - 1));
                        tile->SetGroundType(groundType);
                    }
                    else if (s.find("<elevation>") != std::string::npos)
                    /****************************************************/
                    {
                        auto elevation = std::stof(
                            s.substr(s.find(">") + 1, s.find_last_of("<") - s.find(">") - 1));
                        tile->SetElevation(elevation);
                    }
                    else if (s.find("<roof>") != std::string::npos)
                    /***********************************************/
                    {
                        auto roofType = std::stoi(
                            s.substr(s.find(">") + 1, s.find_last_of("<") - s.find(">") - 1));
                        tile->SetRoofType(roofType);
                    }
                    else if (s.find("<mob>") != std::string::npos)
                    /**********************************************/
                    {
                        std::string s2;
                        int mobType = 0;
                        int spawnX = -1;
                        int spawnY = -1;
                        std::shared_ptr<Mob> newMob = nullptr;
                        while (s2 != "</mob>")
                        /********************/
                        {
                            if (s2.find("<type>") != std::string::npos)
                            /*****************************************/
                            {
                                mobType = std::stoi(s2.substr(
                                    s2.find(">") + 1, s2.find_last_of("<") - s2.find(">") - 1));
                            }
                            else if (s2.find("<spawnX>") != std::string::npos)
                            /**************************************************/
                            {
                                spawnX = std::stoi(s2.substr(
                                    s2.find(">") + 1, s2.find_last_of("<") - s2.find(">") - 1));
                            }
                            else if (s2.find("<spawnY>") != std::string::npos)
                            /**************************************************/
                            {
                                spawnY = std::stoi(s2.substr(
                                    s2.find(">") + 1, s2.find_last_of("<") - s2.find(">") - 1));
                            }
                            mapFile >> s2;
                        }
                        if (mobType != 0)
                        /***************/
                        {
                            tile->SetMob(std::make_shared<Mob>(mobType, spawnX, spawnY));
                            newMapArea->AddMobMirror(tile->GetMob().get(), {x, y});
                        }
                    }
                    else if (s.find("<redVariation>") != std::string::npos)
                    /*******************************************************/
                    {
                        auto redVariation = std::stof(
                            s.substr(s.find(">") + 1, s.find_last_of("<") - s.find(">") - 1));
                        tile->SetRedVariation(redVariation);
                    }
                    else if (s.find("<greenVariation>") != std::string::npos)
                    /*********************************************************/
                    {
                        auto greenVariation = std::stof(
                            s.substr(s.find(">") + 1, s.find_last_of("<") - s.find(">") - 1));
                        tile->SetGreenVariation(greenVariation);
                    }
                    else if (s.find("<blueVariation>") != std::string::npos)
                    /********************************************************/
                    {
                        auto blueVariation = std::stof(
                            s.substr(s.find(">") + 1, s.find_last_of("<") - s.find(">") - 1));
                        tile->SetBlueVariation(blueVariation);
                    }
                    else if (s.find("<objects>") != std::string::npos)
                    /**************************************************/
                    {
                        std::string s2;
                        int objectType = 0;
                        int quantity = 0;
                        while (s2 != "</objects>")
                        /************************/
                        {
                            if (s2.find("</object>") != std::string::npos)
                            /********************************************/
                            {
                                tile->CreateAddObject(objectType, quantity);
                            }
                            else if (s2.find("<type>") != std::string::npos)
                            /************************************************/
                            {
                                objectType = std::stoi(s2.substr(
                                    s2.find(">") + 1, s2.find_last_of("<") - s2.find(">") - 1));
                            }
                            else if (s2.find("<quantity>") != std::string::npos)
                            /****************************************************/
                            {
                                quantity = std::stoi(s2.substr(
                                    s2.find(">") + 1, s2.find_last_of("<") - s2.find(">") - 1));
                            }
                            mapFile >> s2;
                        }
                    }
                    mapFile >> s;
                }
            }
        }
        World::Get().AddMapAreaAtZLevel(0, newMapArea);
        World::Get().CalculateNormals();
        mapFile.close();
        TextOutBox::Get().Print("World loaded with name " + std::string(mapName.data()) + ".");
    }
}
//////////////////////////////////////////////////////////////////////