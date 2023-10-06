//////////////////////////////////////////////////////////////////////
#include "SceneBase.hpp"
#include "Engine/GuiCore/SceneGui.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SceneBase::Pimpl
    /*/////////////////*/
    {
      public:
        std::shared_ptr<SceneGui> sceneGui;
    };

    SceneBase::SceneBase()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void
    SceneBase::Enter()
    /*//////////////////*/
    {
    }

    void
    SceneBase::Update()
    /*///////////////////*/
    {
        p->sceneGui->Update();
        UpdateDerived();
    }

    void
    SceneBase::Render()
    /*///////////////////*/
    {
        Log();
        RenderDerived();
        Log();
        p->sceneGui->Render();
        Log();
        RenderAfterGuiDerived();
    }

    void
    SceneBase::Finalize()
    /*/////////////////////*/
    {
    }

    SceneGui *
    SceneBase::GetSceneGui()
    /*///////////////////////////*/
    {
        return p->sceneGui.get();
    }

    void
    SceneBase::CreateGui()
    /*//////////////////////*/
    {
        SetSceneGui(std::make_shared<SceneGui>());
    }

    void
    SceneBase::RenderAfterGuiDerived()
    /*//////////////////////////////////*/
    {
    }

    void
    SceneBase::SetSceneGui(std::shared_ptr<SceneGui> newSceneGui)
    /*///////////////////////////////////////////////////////////*/
    {
        p->sceneGui = newSceneGui;
    }
}
//////////////////////////////////////////////////////////////////////