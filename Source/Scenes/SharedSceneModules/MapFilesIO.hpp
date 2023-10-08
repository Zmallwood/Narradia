#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MapFilesIO : public Singleton<MapFilesIO> {
      public:
        MapFilesIO();
        void SaveCurrentMapArea(std::string_view mapName);
        void LoadMapArea(std::string_view mapName);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}