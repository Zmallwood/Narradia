//////////////////////////////////////////////////////////////////////
#include "SceneManager.hpp"
#include "Scenes/MapGenerationScene/MapGenerateScene.hpp"
#include "Scenes/GameSaveScene/GameSaveScene.hpp"
#include "Scenes/PlayScene/PlayScene.hpp"
#include "Scenes/EditorScene/EditorScene.hpp"
#include "Scenes/MapLoadScene/MapLoadScene.hpp"
#include "Scenes/MapSaveScene/MapSaveScene.hpp"
#include "Input/MouseInput.hpp"
#include "Scenes/IntroScene/IntroScene.hpp"
#include "Scenes/MainMenuScene/MainMenuScene.hpp"
#include "Scenes/MapTypeSelectionScene/MapTypeSelectionScene.hpp"
#include "Scenes/UserMapSelectionScene/UserMapSelectionScene.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SceneManager::Pimpl
    /*////////////////////*/
    {
      public:
        std::map<SceneNames, std::shared_ptr<SceneBase>> scenes;
        SceneNames currentView = SceneNames::None;
    };

    SceneManager::SceneManager()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void SceneManager::InitializeScenes()
    /*///////////////////////////////*/
    {
        Log();
        p->scenes[SceneNames::Intro] = std::make_shared<IntroScene>();
        Log();
        p->scenes[SceneNames::MainMenu] = std::make_shared<MainMenuScene>();
        Log();
        p->scenes[SceneNames::MapTypeSelection] = std::make_shared<MapTypeSelectionScene>();
        Log();
        p->scenes[SceneNames::UserMapSelection] = std::make_shared<UserMapSelectionScene>();
        Log();
        p->scenes[SceneNames::MapGeneration] = std::make_shared<MapGenerateScene>();
        Log();
        p->scenes[SceneNames::GameSave] = std::make_shared<GameSaveScene>();
        Log();
        p->scenes[SceneNames::Play] = PlayScene::GetPointer();
        Log();
        p->scenes[SceneNames::Editor] = std::make_shared<EditorScene>();
        Log();
        p->scenes[SceneNames::MapSave] = std::make_shared<MapSaveScene>();
        Log();
        p->scenes[SceneNames::MapLoad] = std::make_shared<MapLoadScene>();
        p->currentView = SceneNames::Intro;
    }

    void SceneManager::UpdateCurrentScene()
    /*/////////////////////////////////*/
    {
        Log();
        if (p->scenes.count(p->currentView))
        /**********************************/
        {
            Log();
            p->scenes.at(p->currentView)->Update();
        };
    }

    void SceneManager::RenderCurrentScene() const
    /*///////////////////////////////////////*/
    {
        Log();
        if (p->scenes.count(p->currentView))
        /**********************************/
        {
            Log();
            p->scenes.at(p->currentView)->Render();
        };
        Log();
    }

    void SceneManager::FinalizeCurrentScene()
    /*///////////////////////////////////*/
    {
        if (p->scenes.count(p->currentView))
        /**********************************/
        {
            Log();
            p->scenes.at(p->currentView)->Finalize();
        };
    }

    void SceneManager::ChangeScene(SceneNames newScene)
    /*////////////////////////////////////////*/
    {
        p->currentView = newScene;
        MouseInput::Get().Reset();
        MouseInput::Get().ResetMouseActions();
        p->scenes.at(p->currentView)->Enter();
    }

    SceneNames SceneManager::GetCurrentScene()
    /*///////////////////////////////*/
    {
        return p->currentView;
    }
}
//////////////////////////////////////////////////////////////////////