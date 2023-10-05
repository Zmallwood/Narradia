//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MapArea;
    
    enum class GenerateSteps
    /************************/ {
        CreateBlank = 0,
        GenWater,
        GenElev,
        GenRock,
        GenObjects,
        GenMobs,
        GenNpcs,
        Finalize,
        CalcNormals,
        Completed
    };

    class WorldMapGenerate
    /*********************/ {
      public:
        void DoGenerationStep(GenerateSteps);

      private:
        void AddElevation();
        void AddMobs();
        void AddObjects();
        void AddWater();
        void AddWaterRivers();
        void AddObjects(std::string_view, int, std::string_view);
        void GenerateColorVariations();
        std::shared_ptr<MapArea> newMapArea;
    };
}
//////////////////////////////////////////////////////////////////////