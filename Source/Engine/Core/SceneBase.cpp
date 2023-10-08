#include "SceneBase.hpp"
#include "Engine/GuiCore/SceneGui.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SceneBase::Pimpl {
      public:
        std::shared_ptr<SceneGui> scene_gui_;
    };

    SceneBase::SceneBase()
        : p(std::make_shared<Pimpl>()) {
    }

    void SceneBase::Enter() {
    }

    void SceneBase::Update() {
        p->scene_gui_->Update();
        UpdateDerived();
    }

    void SceneBase::Render() {
        Log();
        RenderDerived();
        Log();
        p->scene_gui_->Render();
        Log();
        RenderAfterGuiDerived();
    }

    void SceneBase::Finalize() {
    }

    SceneGui *SceneBase::GetSceneGui() {
        return p->scene_gui_.get();
    }

    void SceneBase::CreateGui() {
        SetSceneGui(std::make_shared<SceneGui>());
    }

    void SceneBase::RenderAfterGuiDerived() {
    }

    void SceneBase::SetSceneGui(std::shared_ptr<SceneGui> newSceneGui) {
        p->scene_gui_ = newSceneGui;
    }
}