#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MapArea;

    class World : public Singleton<World> {
      public:
        World();
        void AddMapAreaAtZLevel(int, std::shared_ptr<MapArea>);
        void RemoveMapAreaAtZLevel(int);
        MapArea *GetMapAreaAtZLevel(int);
        MapArea *GetCurrentMapArea();
        std::vector<MapArea *> GetAllMapAreas();
        void ResetColorVariations();
        void ApplyDefaultColorVariations();
        void GenerateRandomColorVariations();
        void CalculateNormals(bool = false);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}