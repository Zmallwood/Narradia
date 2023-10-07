#include "GameLoop.hpp"
#include "Audio.hpp"
#include "Cursor.hpp"
#include "EventPoller.hpp"
#include "GameEngine.hpp"
#include "Graphics/Graphics.hpp"
#include "Input/MouseInput.hpp"
#include "SceneManager.hpp"
#include "TextOutBox.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void
    GameLoop::Run() const
    /*/////////////////*/
    {
        Log();
        World::Create();
        Log();
        SceneManager::Get()->InitializeScenes();
        Log();
        EventPoller event_poller;
        while (GameEngine::Get()->IsRunning())
        /*********************************/
        {
            Log();
            Cursor::Get()->Reset();
            TextOutBox::Get()->Update();
            Log();
            Graphics::Get()->ClearCanvas();
            Log();
            SceneManager::Get()->UpdateCurrentScene();
            Log();
            event_poller.PollEvents();
            Log();
            MouseInput::Get()->PerformMouseActions();
            Log();
            SceneManager::Get()->RenderCurrentScene();
            Log();
            SceneManager::Get()->FinalizeCurrentScene();
            Log();
            TextOutBox::Get()->Render();
            Log();
            Cursor::Get()->Render();
            Log();
            Graphics::Get()->PresentCanvas();
        }
        Log();
        Graphics::Get()->Cleanup();
        Log();
        Audio::Get()->Cleanup();
    }
}