//////////////////////////////////////////////////////////////////////
#include "Core.hpp"
#include "Engine/GuiCore.hpp"
#include "Engine/Core/Graphics/Rendering/Rendering.hpp"
#include "SceneManager.hpp"
#include "Graphics/Graphics.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    Size GetCanvasSize()
    /*////////////////*/
    {
        int width;
        int height;
        SDL_GetWindowSize(Graphics::Get().GetWindow(), &width, &height);
        return {width, height};
    }

    float GetAspectRatio()
    /*//////////////////*/
    {
        auto canvasSize = GetCanvasSize();
        return static_cast<float>(canvasSize.width) / canvasSize.height;
    }

    float ConvertWidthToHeight(float width)
    /*///////////////////////////////////*/
    {
        return width * GetAspectRatio();
    }

    std::string_view GetCurrentTime()
    /*/////////////////////////////*/
    {
        time_t now = time(0);
        char buffer[80];
        auto pTstruct = localtime(&now);
        strftime(buffer, sizeof(buffer), "%X", pTstruct);
        return buffer;
    }

    std::string_view GetCurrentDateTime()
    /*/////////////////////////////////*/
    {
        time_t now = time(0);
        char buffer[80];
        auto pTstruct = localtime(&now);
        strftime(buffer, sizeof(buffer), "%Y.%m.%d.%H.%M.%S", pTstruct);
        return buffer;
    }

    const int Hash(const std::string_view &text)
    /*////////////////////////////////////////*/
    {
        return std::hash<std::string_view>{}(text);
    }

    Point2 GetMousePositionPx()
    /*///////////////////////*/
    {
        int x;
        int y;
        SDL_GetMouseState(&x, &y);
        return {x, y};
    }

    Point2F GetMousePositionF()
    /*///////////////////////*/
    {
        auto canvasSize = GetCanvasSize();
        auto mousePosPx = GetMousePositionPx();
        auto x = static_cast<float>(mousePosPx.x) / canvasSize.width;
        auto y = static_cast<float>(mousePosPx.y) / canvasSize.height;
        return {x, y};
    }

    class KeyboardInput::Pimpl
    /*//////////////////////*/
    {
      public:
        std::string textInput;
        std::set<SDL_Keycode> pressedKeys;
        std::set<SDL_Keycode> firedKeys;
    };

    KeyboardInput::KeyboardInput()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void KeyboardInput::KeyDown(SDL_Keycode key)
    /*////////////////////////////////////////*/
    {
        if (p->pressedKeys.count(key) == 0)
            p->firedKeys.insert(key);
        p->pressedKeys.insert(key);
    }

    void KeyboardInput::KeyUp(SDL_Keycode key)
    /*//////////////////////////////////////*/
    {
        p->pressedKeys.erase(key);
    }

    bool KeyboardInput::AnyKeyHasBeenFired() const
    /*//////////////////////////////////////////*/
    {
        return p->firedKeys.size() > 0;
    }

    bool KeyboardInput::KeyIsPressed(SDL_Keycode key) const
    /*///////////////////////////////////////////////////*/
    {
        return p->pressedKeys.count(key) > 0;
    }

    bool KeyboardInput::KeyHasBeenFiredPickResult(SDL_Keycode key)
    /*//////////////////////////////////////////////////////////*/
    {
        auto result = p->firedKeys.count(key) > 0;
        p->firedKeys.erase(key);
        return result;
    }

    std::string_view KeyboardInput::PickTextInput()
    /*///////////////////////////////////////////*/
    {
        auto result = p->textInput;
        p->textInput = "";
        return result;
    }

    void KeyboardInput::ResetTextInput()
    /*////////////////////////////////*/
    {
        p->textInput = "";
    }

    void KeyboardInput::AppendTextInput(std::string_view toAppend)
    /*//////////////////////////////////////////////////////////*/
    {
        p->textInput.append(toAppend);
    }

    class MouseButton::Pimpl
    /*////////////////////*/
    {
      public:
        static constexpr int defaultClickSpeed = 200;
        bool isPressed = false;
        Uint64 ticksButtonDown = 0;
        int clickDuration = 0;
        bool hasBeenFired = false;
        bool hasBeenReleased = false;
        MouseActionManager actionMngr;
    };

    MouseButton::MouseButton()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void MouseButton::Reset()
    /*/////////////////////*/
    {
        p->isPressed = false;
    }

    void MouseButton::PressDown()
    /*/////////////////////////*/
    {
        p->isPressed = true;
        p->hasBeenFired = true;
        p->hasBeenReleased = false;
        p->clickDuration = 0;
        p->ticksButtonDown = SDL_GetTicks();
    }

    void MouseButton::Release()
    /*///////////////////////*/
    {
        p->isPressed = false;
        p->hasBeenReleased = true;
        p->clickDuration = SDL_GetTicks() - p->ticksButtonDown;
    }

    int MouseButton::GetPressDuration() const
    /*/////////////////////////////////////*/
    {
        return SDL_GetTicks() - p->ticksButtonDown;
    }

    int MouseButton::ClickDurationPickResult()
    /*//////////////////////////////////////*/
    {
        auto result = p->clickDuration;
        p->clickDuration = Pimpl::defaultClickSpeed;
        return result;
    }

    int MouseButton::ClickDurationPeekResult() const
    /*////////////////////////////////////////////*/
    {
        return p->clickDuration;
    }

    void MouseButton::PerformMouseAction()
    /*//////////////////////////////////*/
    {
        Log();
        if (p->hasBeenFired)
            p->actionMngr.PerformFiredActions(p->ticksButtonDown, p->isPressed, p->hasBeenFired);
        else
            p->actionMngr.ClearFiredActions();
        Log();
        if (p->hasBeenReleased)
            p->actionMngr.PerformReleasedActions(p->ticksButtonDown);
        else
            p->actionMngr.ClearReleasedActions();
    }

    void MouseButton::AddFiredAction(
        std::string_view uniqueName, std::function<void()> action, float priority, int delay,
        bool ensureIsExec)
    /*/////////////////////////////////////////////////////////////////////////////////////*/
    {
        if (p->hasBeenFired == true)
            return;
        auto id = Hash(uniqueName);
        if (p->actionMngr.FiredActionsContains(id))
            return;
        p->actionMngr.AddFiredAction(
            id, {action, priority, static_cast<Uint64>(delay), ensureIsExec});
    }

    void MouseButton::AddReleasedAction(
        std::string_view uniqueName, std::function<void()> action, float priority, int delay,
        bool ensureIsExec)
    /*/////////////////////////////////////////////////////////////////////////////////////*/
    {
        if (p->hasBeenReleased == true)
            return;
        auto id = Hash(uniqueName);
        if (p->actionMngr.ReleasedActionsContains(id))
            return;
        p->actionMngr.AddReleasedAction(
            id, {action, priority, static_cast<Uint64>(delay), ensureIsExec});
    }

    void MouseButton::ResetActions()
    /*////////////////////////////*/
    {
        p->actionMngr.ClearFiredActions();
        p->hasBeenFired = false;
        p->actionMngr.ClearReleasedActions();
        p->hasBeenReleased = false;
    }

    const int MouseButton::GetDefaultClickSpeed()
    /*/////////////////////////////////////////*/
    {
        return Pimpl::defaultClickSpeed;
    }

    class MouseInput::Pimpl
    /*///////////////////*/
    {
      public:
        Point2 motionDelta = {0, 0};
        int mouseWheelDelta = 0;
        MouseButton leftButton;
        MouseButton middleButton;
        MouseButton rightButton;
    };

    MouseInput::MouseInput()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void MouseInput::Reset()
    /*////////////////////*/
    {
        p->leftButton.Reset();
        p->middleButton.Reset();
        p->rightButton.Reset();
        p->mouseWheelDelta = 0;
        p->motionDelta = {0, 0};
        ResetMouseActions();
    }

    void MouseInput::MouseDown(Uint8 mouseButton)
    /*/////////////////////////////////////////*/
    {
        switch (mouseButton)
        /******************/
        {
        case SDL_BUTTON_LEFT:
            /***************/
            p->leftButton.PressDown();
            break;
        case SDL_BUTTON_MIDDLE:
            /*****************/
            p->middleButton.PressDown();
            break;
        case SDL_BUTTON_RIGHT:
            /****************/
            p->rightButton.PressDown();
            break;
        }
    }

    void MouseInput::MouseUp(Uint8 mouseButton)
    /*///////////////////////////////////////*/
    {
        switch (mouseButton)
        /******************/
        {
        case SDL_BUTTON_LEFT:
            /***************/
            p->leftButton.Release();
            break;
        case SDL_BUTTON_MIDDLE:
            /*****************/
            p->middleButton.Release();
            break;
        case SDL_BUTTON_RIGHT:
            /****************/
            p->rightButton.Release();
            break;
        }
    }

    void MouseInput::MouseWheel(int deltaScroll)
    /*////////////////////////////////////////*/
    {
        p->mouseWheelDelta += deltaScroll;
    }

    void MouseInput::MouseMove(Point2 motionDelta_)
    /*///////////////////////////////////////////*/
    {
        p->motionDelta = motionDelta_;
    }

    int MouseInput::MouseWheelDeltaPickResult()
    /*///////////////////////////////////////*/
    {
        auto resultMouseWheelDelta = p->mouseWheelDelta;
        p->mouseWheelDelta = 0;
        return resultMouseWheelDelta;
    }

    void MouseInput::PerformMouseActions()
    /*//////////////////////////////////*/
    {
        Log();
        p->leftButton.PerformMouseAction();
        Log();
        p->middleButton.PerformMouseAction();
        Log();
        p->rightButton.PerformMouseAction();
    }

    void MouseInput::ResetMouseActions()
    /*////////////////////////////////*/
    {
        p->leftButton.ResetActions();
        p->middleButton.ResetActions();
        p->rightButton.ResetActions();
    }

    Point2 MouseInput::GetMotionDeltaPickResults()
    /*//////////////////////////////////////////*/
    {
        auto result = p->motionDelta;
        p->motionDelta = {0, 0};
        return result;
    }

    MouseButton &MouseInput::GetLeftButton()
    /*////////////////////////////////////*/
    {
        return p->leftButton;
    }

    MouseButton &MouseInput::GetMiddleButton()
    /*//////////////////////////////////////*/
    {
        return p->middleButton;
    }

    MouseButton &MouseInput::GetRightButton()
    /*/////////////////////////////////////*/
    {
        return p->rightButton;
    }

    class MouseActionManager::Pimpl
    /*///////////////////////////*/
    {
      public:
        std::map<int, MouseAction> firedActions;
        std::map<int, MouseAction> releasedActions;
    };

    MouseActionManager::MouseActionManager()
        : p(std::make_shared<Pimpl>())
    /*////////////////////////////////////*/
    {
    }

    void
    MouseActionManager::PerformFiredActions(int ticksButtonDown, bool isPressed, bool &hasBeenFired)
    /*////////////////////////////////////////////////////////////////////////////////////////////*/
    {
        std::function<void()> firedAction;
        float firedPriority = -1;
        Uint64 ticksPerform = 0;
        int key = 0;
        bool highestPrioNotReady = false;
        std::set<int> firedToDelete;
        Log();
        for (auto it = p->firedActions.begin(); it != p->firedActions.end();)
        /*******************************************************************/
        {
            if (it->second.priority > firedPriority)
            /**************************************/
            {
                firedPriority = it->second.priority;
                firedAction = it->second.action;
                ticksPerform = ticksButtonDown + it->second.delay;
                key = it->first;
                auto diff = (int)(ticksPerform - SDL_GetTicks());
                if (diff > 0)
                /***********/
                {
                    highestPrioNotReady = true;
                    it++;
                    continue;
                }
            }
            if (false == it->second.ensureIsExec)
                firedToDelete.insert(it->first);
            it++;
        }
        Log();
        if (highestPrioNotReady)
        /**********************/
        {
            int prevPriority = -1;
            for (auto it = p->firedActions.begin(); it != p->firedActions.end();)
            /*******************************************************************/
            {
                if (it->second.priority < firedPriority && it->second.priority > prevPriority)
                /****************************************************************************/
                {
                    key = it->first;
                    firedAction = it->second.action;
                    prevPriority = ticksButtonDown + it->second.delay;
                    ticksPerform = ticksButtonDown + it->second.delay;
                    firedPriority = it->second.priority;
                }
                it++;
            }
        }
        Log();
        if (SDL_GetTicks() >= ticksPerform)
        /*********************************/
        {
            if (firedAction)
            /**************/
            {
                if (isPressed)
                    firedAction();
                p->firedActions.erase(key);
            }
        }
        Log();
        for (auto key : firedToDelete)
            p->firedActions.erase(key);
        Log();
        if (p->firedActions.size() == 0 && isPressed == false)
            hasBeenFired = false;
    }

    void MouseActionManager::PerformReleasedActions(int ticksButtonDown)
    /*////////////////////////////////////////////////////////////////*/
    {
        std::function<void()> releasedAction;
        float releasedPriority = -1;
        Uint64 ticksPerform = 0;
        int key = 0;
        bool highestPrioNotReady = false;
        std::set<int> releasedToDelete;
        Log();
        for (auto it = p->releasedActions.begin(); it != p->releasedActions.end();)
        /*************************************************************************/
        {
            Log();
            if (it->second.priority > releasedPriority)
            /*****************************************/
            {
                releasedPriority = it->second.priority;
                releasedAction = it->second.action;
                ticksPerform = ticksButtonDown + it->second.delay;
                key = it->first;
                auto diff = (int)(ticksPerform - SDL_GetTicks());
                Log();
                if (diff > 0)
                /***********/
                {
                    highestPrioNotReady = true;
                    it++;
                    continue;
                }
            }
            Log();
            if (false == it->second.ensureIsExec)
                releasedToDelete.insert(it->first);
            it++;
        }
        Log();
        if (highestPrioNotReady)
        /**********************/
        {
            int prevPriority = -1;
            for (auto it = p->releasedActions.begin(); it != p->releasedActions.end();)
            /*************************************************************************/
            {
                if (it->second.priority < releasedPriority && it->second.priority > prevPriority)
                /*******************************************************************************/
                {
                    key = it->first;
                    releasedAction = it->second.action;
                    prevPriority = ticksButtonDown + it->second.delay;
                    ticksPerform = ticksButtonDown + it->second.delay;
                    releasedPriority = it->second.priority;
                }
                it++;
            }
        }
        Log();
        if (SDL_GetTicks() >= ticksPerform)
        /*********************************/
        {
            Log();
            if (releasedAction)
            /*****************/
            {
                Log();
                releasedAction();
                Log();
                p->releasedActions.erase(key);
            }
        }
        Log();
        for (auto key : releasedToDelete)
            p->releasedActions.erase(key);
    }

    void MouseActionManager::ClearFiredActions()
    /*////////////////////////////////////////*/
    {
        p->firedActions.clear();
    }

    void MouseActionManager::ClearReleasedActions()
    /*///////////////////////////////////////////*/
    {
        p->releasedActions.clear();
    }

    bool MouseActionManager::FiredActionsContains(int actionId)
    /*///////////////////////////////////////////////////////*/
    {
        return p->firedActions.count(actionId);
    }

    bool MouseActionManager::ReleasedActionsContains(int actionId)
    /*//////////////////////////////////////////////////////////*/
    {
        return p->releasedActions.count(actionId);
    }

    void MouseActionManager::AddFiredAction(int actionId, MouseAction action)
    /*/////////////////////////////////////////////////////////////////////*/
    {
        p->firedActions.insert({actionId, action});
    }

    void MouseActionManager::AddReleasedAction(int actionId, MouseAction action)
    /*////////////////////////////////////////////////////////////////////////*/
    {
        p->releasedActions.insert({actionId, action});
    }
}
//////////////////////////////////////////////////////////////////////