//////////////////////////////////////////////////////////////////////
#include "GameLoop.hpp"
#include "World/World.hpp"
#include "GameEngine.hpp"
#include "EventPoller.hpp"
#include "SceneManager.hpp"
#include "Cursor.hpp"
#include "TextOutBox.hpp"
#include "Graphics/Graphics.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void GameLoop::Run() const
    /*//////////////////////*/
    {
        Log();
        World::Create();
        Log();
        SceneManager::Get().InitializeScenes();
        Log();
        EventPoller eventPoller;
        while (GameEngine::Get().IsRunning())
        /*********************************/
        {
            Log();
            Cursor::Get().Reset();
            TextOutBox::Get().Update();
            Log();
            Graphics::Get().ClearCanvas();
            Log();
            SceneManager::Get().UpdateCurrentScene();
            Log();
            eventPoller.PollEvents();
            Log();
            MouseInput::Get().PerformMouseActions();
            Log();
            SceneManager::Get().RenderCurrentScene();
            Log();
            SceneManager::Get().FinalizeCurrentScene();
            Log();
            TextOutBox::Get().Render();
            Log();
            Cursor::Get().Render();
            Log();
            Graphics::Get().PresentCanvas();
        }
        Log();
        Graphics::Get().Cleanup();
        Log();
        Audio::Get().Cleanup();
    }
}
//////////////////////////////////////////////////////////////////////