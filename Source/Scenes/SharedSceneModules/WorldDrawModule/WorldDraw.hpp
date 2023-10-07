//////////////////////////////////////////////////////////////////////
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
        WorldDrawModes worldViewMode = WorldDrawModes::Play;
        friend class PlayScene;
        friend class EditorScene;
    };
}
//////////////////////////////////////////////////////////////////////