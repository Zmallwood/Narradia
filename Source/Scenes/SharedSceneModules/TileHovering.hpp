//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class TileHovering : public Singleton<TileHovering>
    /*///////////////////////////////////////////////*/
    {
      public:
        TileHovering();
        Point2 hoveredTile = {-1, -1};

      private:
        void Update();
        void Render();
        RenderId glIdTextHoveredObject = 0;
        int hoveredObjectHash = 0;
        friend class PlayScene;
        friend class EditorScene;
    };
}
//////////////////////////////////////////////////////////////////////