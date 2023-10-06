//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SceneGui;

    class SceneBase
    /*//////////*/
    {
      public:
        SceneBase();
        virtual void Enter();
        void Update();
        void Render();
        virtual void Finalize();
        SceneGui *GetSceneGui();

      protected:
        virtual void CreateGui();
        virtual void UpdateDerived() = 0;
        virtual void RenderDerived() = 0;
        virtual void RenderAfterGuiDerived();
        void SetSceneGui(std::shared_ptr<SceneGui>);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}
//////////////////////////////////////////////////////////////////////