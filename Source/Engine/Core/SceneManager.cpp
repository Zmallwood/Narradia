#include "SceneManager.hpp"
#include "Input/MouseInput.hpp"
#include "Scenes/EditorScene/EditorScene.hpp"
#include "Scenes/GameSaveScene/GameSaveScene.hpp"
#include "Scenes/IntroScene/IntroScene.hpp"
#include "Scenes/MainMenuScene/MainMenuScene.hpp"
#include "Scenes/MapGenerationScene/MapGenerationScene.hpp"
#include "Scenes/MapLoadScene/MapLoadScene.hpp"
#include "Scenes/MapSaveScene/MapSaveScene.hpp"
#include "Scenes/MapTypeSelectionScene/MapTypeSelectionScene.hpp"
#include "Scenes/PlayScene/PlayScene.hpp"
#include "Scenes/UserMapSelectionScene/UserMapSelectionScene.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SceneManager::Pimpl {
      public:
        std::map<SceneNames, std::shared_ptr<SceneBase>> scenes_;
        SceneNames current_scene_ = SceneNames::None;
    };

    SceneManager::SceneManager()
        : p(std::make_shared<Pimpl>()) {
    }

    void SceneManager::InitializeScenes() {
        Log();
        p->scenes_[SceneNames::Intro] = std::make_shared<IntroScene>();
        Log();
        p->scenes_[SceneNames::MainMenu] = std::make_shared<MainMenuScene>();
        Log();
        p->scenes_[SceneNames::MapTypeSelection] = std::make_shared<MapTypeSelectionScene>();
        Log();
        p->scenes_[SceneNames::UserMapSelection] = std::make_shared<UserMapSelectionScene>();
        Log();
        p->scenes_[SceneNames::MapGeneration] = std::make_shared<MapGenerationScene>();
        Log();
        p->scenes_[SceneNames::GameSave] = std::make_shared<GameSaveScene>();
        Log();
        p->scenes_[SceneNames::Play] = PlayScene::Get();
        Log();
        p->scenes_[SceneNames::Editor] = std::make_shared<EditorScene>();
        Log();
        p->scenes_[SceneNames::MapSave] = std::make_shared<MapSaveScene>();
        Log();
        p->scenes_[SceneNames::MapLoad] = std::make_shared<MapLoadScene>();
        p->current_scene_ = SceneNames::Intro;
    }

    void SceneManager::UpdateCurrentScene() {
        Log();
        if (p->scenes_.count(p->current_scene_)) {
            Log();
            p->scenes_.at(p->current_scene_)->Update();
        };
    }

    void SceneManager::RenderCurrentScene() const {
        Log();
        if (p->scenes_.count(p->current_scene_)) {
            Log();
            p->scenes_.at(p->current_scene_)->Render();
        };
        Log();
    }

    void SceneManager::FinalizeCurrentScene() {
        if (p->scenes_.count(p->current_scene_)) {
            Log();
            p->scenes_.at(p->current_scene_)->Finalize();
        };
    }

    void SceneManager::ChangeScene(SceneNames newScene) {
        p->current_scene_ = newScene;
        MouseInput::Get()->Reset();
        MouseInput::Get()->ResetMouseActions();
        p->scenes_.at(p->current_scene_)->Enter();
    }

    SceneNames SceneManager::GetCurrentScene() {
        return p->current_scene_;
    }
}