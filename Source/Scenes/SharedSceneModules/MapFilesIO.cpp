#include "MapFilesIO.hpp"
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
    class MapFilesIO::Pimpl {
      public:
        static constexpr std::string_view kRelMapsPath = "Maps";
    };

    MapFilesIO::MapFilesIO()
        : p(std::make_shared<Pimpl>()) {
    }

    void MapFilesIO::SaveCurrentMapArea(std::string_view mapName) {
        std::filesystem::create_directory(
            std::string(SDL_GetBasePath()) + Pimpl::kRelMapsPath.data());
        std::filesystem::remove(
            std::string(SDL_GetBasePath()) + Pimpl::kRelMapsPath.data() + "/" + mapName.data() +
            ".map");
        std::ofstream map_file;
        map_file.open(
            std::string(SDL_GetBasePath()) + Pimpl::kRelMapsPath.data() + "/" + mapName.data() +
                ".map",
            std::ofstream::out | std::ofstream::trunc);
        auto map_area = World::Get()->GetCurrentMapArea();
        for (auto y = 0; y < MapArea::GetMapSize().height; y++) {
            for (auto x = 0; x < MapArea::GetMapSize().width; x++) {
                auto tile = map_area->GetTile(x, y);
                map_file << "<tile>\n";
                map_file << "   <ground>" << tile->GetGroundType() << "</ground>\n";
                map_file << "   <elevation>" << tile->GetElevation() << "</elevation>\n";
                map_file << "   <roof>" << tile->GetRoofType() << "</roof>\n";
                map_file << "   <mob>\n";
                if (tile->GetMob()) {
                    map_file << "<type>" << tile->GetMob()->GetMobType() << "</type>\n";
                    map_file << "<spawnX>" << tile->GetMob()->GetSpawnX() << "</spawnX>\n";
                    map_file << "<spawnY>" << tile->GetMob()->GetSpawnY() << "</spawnY>\n";
                }
                else {
                    map_file << "<type>" << 0 << "</type>";
                }
                map_file << "   </mob>\n";
                map_file << "   <redVariation>" << tile->GetRedVariation() << "</redVariation>\n";
                map_file << "   <greenVariation>" << tile->GetGreenVariation()
                         << "</greenVariation>\n";
                map_file << "   <blueVariation>" << tile->GetBlueVariation()
                         << "</blueVariation>\n";
                map_file << "   <objects>\n";
                for (auto &entry : tile->GetObjects().list_) {
                    map_file << "      <object>\n";
                    map_file << "         <type>" << entry->GetObjectType() << "</type>\n";
                    map_file << "         <quantity>" << entry->GetQuantity() << "</quantity>\n";
                    map_file << "      </object>\n";
                }
                map_file << "   </objects>\n";
                map_file << "</tile>\n";
            }
        }
        map_file.close();
        TextOutBox::Get()->Print("World saved with name " + std::string(mapName.data()) + ".");
    }

    void MapFilesIO::LoadMapArea(std::string_view mapName) {
        std::ifstream map_file;
        map_file.open(
            std::string(SDL_GetBasePath()) + Pimpl::kRelMapsPath.data() + "/" + mapName.data() +
            ".map");
        World::Get()->RemoveMapAreaAtZLevel(0);
        auto new_map_area = std::make_shared<MapArea>();
        new_map_area->Create();
        for (auto y = 0; y < MapArea::GetMapSize().height; y++) {
            for (auto x = 0; x < MapArea::GetMapSize().width; x++) {
                auto tile = new_map_area->GetTile(x, y);
                std::string s;
                map_file >> s;
                while (s != "</tile>") {
                    if (s.find("<ground>") != std::string::npos) {
                        auto ground_type = std::stoi(
                            s.substr(s.find(">") + 1, s.find_last_of("<") - s.find(">") - 1));
                        tile->SetGroundType(ground_type);
                    }
                    else if (s.find("<elevation>") != std::string::npos) {
                        auto elevation = std::stof(
                            s.substr(s.find(">") + 1, s.find_last_of("<") - s.find(">") - 1));
                        tile->SetElevation(elevation);
                    }
                    else if (s.find("<roof>") != std::string::npos) {
                        auto roof_type = std::stoi(
                            s.substr(s.find(">") + 1, s.find_last_of("<") - s.find(">") - 1));
                        tile->SetRoofType(roof_type);
                    }
                    else if (s.find("<mob>") != std::string::npos) {
                        std::string s2;
                        int mob_type = 0;
                        int spawn_x = -1;
                        int spawn_y = -1;
                        while (s2 != "</mob>") {
                            if (s2.find("<type>") != std::string::npos) {
                                mob_type = std::stoi(s2.substr(
                                    s2.find(">") + 1, s2.find_last_of("<") - s2.find(">") - 1));
                            }
                            else if (s2.find("<spawnX>") != std::string::npos) {
                                spawn_x = std::stoi(s2.substr(
                                    s2.find(">") + 1, s2.find_last_of("<") - s2.find(">") - 1));
                            }
                            else if (s2.find("<spawnY>") != std::string::npos) {
                                spawn_y = std::stoi(s2.substr(
                                    s2.find(">") + 1, s2.find_last_of("<") - s2.find(">") - 1));
                            }
                            map_file >> s2;
                        }
                        if (mob_type != 0) {
                            tile->SetMob(std::make_shared<Mob>(mob_type, spawn_x, spawn_y));
                            new_map_area->AddMobMirror(tile->GetMob().get(), {x, y});
                        }
                    }
                    else if (s.find("<redVariation>") != std::string::npos) {
                        auto red_variation = std::stof(
                            s.substr(s.find(">") + 1, s.find_last_of("<") - s.find(">") - 1));
                        tile->SetRedVariation(red_variation);
                    }
                    else if (s.find("<greenVariation>") != std::string::npos) {
                        auto green_variation = std::stof(
                            s.substr(s.find(">") + 1, s.find_last_of("<") - s.find(">") - 1));
                        tile->SetGreenVariation(green_variation);
                    }
                    else if (s.find("<blueVariation>") != std::string::npos) {
                        auto blue_variation = std::stof(
                            s.substr(s.find(">") + 1, s.find_last_of("<") - s.find(">") - 1));
                        tile->SetBlueVariation(blue_variation);
                    }
                    else if (s.find("<objects>") != std::string::npos) {
                        std::string s2;
                        int object_type = 0;
                        int quantity = 0;
                        while (s2 != "</objects>") {
                            if (s2.find("</object>") != std::string::npos) {
                                tile->CreateAddObject(object_type, quantity);
                            }
                            else if (s2.find("<type>") != std::string::npos) {
                                object_type = std::stoi(s2.substr(
                                    s2.find(">") + 1, s2.find_last_of("<") - s2.find(">") - 1));
                            }
                            else if (s2.find("<quantity>") != std::string::npos) {
                                quantity = std::stoi(s2.substr(
                                    s2.find(">") + 1, s2.find_last_of("<") - s2.find(">") - 1));
                            }
                            map_file >> s2;
                        }
                    }
                    map_file >> s;
                }
            }
        }
        World::Get()->AddMapAreaAtZLevel(0, new_map_area);
        World::Get()->CalculateNormals();
        map_file.close();
        TextOutBox::Get()->Print("World loaded with name " + std::string(mapName.data()) + ".");
    }
}