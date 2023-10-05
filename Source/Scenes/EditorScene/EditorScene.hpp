//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class EditorScene : public SceneBase, public Singleton<EditorScene>
    /*////////////////////////////////////////////////////////////*/ {
      public:
        EditorScene();

      private:
        void Enter() override;
        void UpdateDerived() override;
        void RenderDerived() override;
        void RenderAfterGuiDerived() override;
    };
}
//////////////////////////////////////////////////////////////////////