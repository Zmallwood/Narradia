#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MapArea;
    
    enum class GenerationSteps
    /************************/
    {
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

    class WorldMapGenerator
    /*********************/
    {
      public:
        void DoGenerationStep(GenerationSteps);

      private:
        void AddElevation();
        void AddMobs();
        void AddObjects();
        void AddWater();
        void AddWaterRivers();
        void AddObjects(std::string_view, int, std::string_view);
        void GenerateColorVariations();

        std::shared_ptr<MapArea> new_map_area_;
    };
}