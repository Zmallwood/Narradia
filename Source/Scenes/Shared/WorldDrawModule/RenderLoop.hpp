//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MapArea;
    class Tile;

    struct ModelRenderParameters
    /*////////////////////////*/
    {
        std::vector<Point3F> positions;
        std::vector<float> rotations;
        std::vector<float> scalings;
        std::vector<float> brightnesses;
        std::vector<glm::vec3> colorMods;
    };
    
    class RenderLoop
    /*////////////*/
    {
      public:
        RenderLoop(std::function<void()>);
        void operator()();
        inline static int currX = -1;
        inline static int currY = -1;
        inline static MapArea *currMapArea = nullptr;
        inline static Point2 currTileCoord = {-1, -1};
        inline static Tile *currTile = nullptr;
        inline static VertexTile currVertTile;
        inline static Point2 currCoord10;
        inline static Point2 currCoord11;
        inline static Point2 currCoord01;
        inline static bool currThisTileClaimedByPlayer = false;
        inline static bool currEastTileClaimedByPlayer = false;
        inline static bool currSouthTileClaimedByPlayer = false;
        inline static int currDistSquared = 0;
        inline static float currTileAvgElev = 0.0f;
        inline static float currElev00 = 0.0f;
        inline static float currElev10 = 0.0f;
        inline static float currElev11 = 0.0f;
        inline static float currElev01 = 0.0f;
        inline static float playerTileAvgElev = 0.0f;
        inline static float playerElev00 = 0.0f;
        inline static float playerElev10 = 0.0f;
        inline static float playerElev11 = 0.0f;
        inline static float playerElev01 = 0.0f;
        inline static std::map<int, ModelRenderParameters> data1;
        inline static std::map<int, ModelRenderParameters> data2;

      private:
        std::function<void()> action;
    };
}
//////////////////////////////////////////////////////////////////////