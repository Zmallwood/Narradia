//////////////////////////////////////////////////////////////////////
#include "MouseActionManager.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
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

    void
    MouseActionManager::PerformReleasedActions(int ticksButtonDown)
    /*///////////////////////////////////////////////////////////*/
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

    void
    MouseActionManager::ClearFiredActions()
    /*///////////////////////////////////*/
    {
        p->firedActions.clear();
    }

    void
    MouseActionManager::ClearReleasedActions()
    /*//////////////////////////////////////*/
    {
        p->releasedActions.clear();
    }

    bool
    MouseActionManager::FiredActionsContains(int actionId)
    /*//////////////////////////////////////////////////*/
    {
        return p->firedActions.count(actionId);
    }

    bool
    MouseActionManager::ReleasedActionsContains(int actionId)
    /*/////////////////////////////////////////////////////*/
    {
        return p->releasedActions.count(actionId);
    }

    void
    MouseActionManager::AddFiredAction(int actionId, MouseAction action)
    /*////////////////////////////////////////////////////////////////*/
    {
        p->firedActions.insert({actionId, action});
    }

    void
    MouseActionManager::AddReleasedAction(int actionId, MouseAction action)
    /*///////////////////////////////////////////////////////////////////*/
    {
        p->releasedActions.insert({actionId, action});
    }
}