#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    enum class WorldDrawModes
    /*/////////////////////*/
    {
        Play,
        Editor
    };
    
    class WorldDraw : public Singleton<WorldDraw>
    /*/////////////////////////////////////////*/
    {
      public:
        WorldDraw();

      private:
        void Update();
        void Render();
        void EnablePlayMode();
        void EnableMapEditorMode();

        WorldDrawModes current_world_draw_mode_ = WorldDrawModes::Play;
        friend class PlayScene;
        friend class EditorScene;
    };
}