#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class TileHovering : public Singleton<TileHovering>
    /*///////////////////////////////////////////////*/
    {
      public:
        TileHovering();

        Point2 hovered_tile_ = {-1, -1};

      private:
        void Update();
        void Render();
        
        RenderId rendid_hovered_object_text_ = 0;
        int hovered_object_hash_ = 0;

        friend class PlayScene;
        friend class EditorScene;
    };
}