#include "MouseActionManager.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MouseActionManager::Pimpl
    /*///////////////////////////*/
    {
      public:
        std::map<int, MouseAction> fired_actions_;
        std::map<int, MouseAction> released_actions_;
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
        std::function<void()> fired_action;
        float fired_priority = -1;
        Uint64 ticks_perform = 0;
        int key = 0;
        bool highest_prio_not_ready = false;
        std::set<int> fired_to_delete;
        Log();
        for (auto it = p->fired_actions_.begin(); it != p->fired_actions_.end();)
        /*******************************************************************/
        {
            if (it->second.priority > fired_priority)
            /**************************************/
            {
                fired_priority = it->second.priority;
                fired_action = it->second.action;
                ticks_perform = ticksButtonDown + it->second.delay;
                key = it->first;
                auto diff = (int)(ticks_perform - SDL_GetTicks());
                if (diff > 0)
                /***********/
                {
                    highest_prio_not_ready = true;
                    it++;
                    continue;
                }
            }
            if (false == it->second.ensureIsExec)
                fired_to_delete.insert(it->first);
            it++;
        }
        Log();
        if (highest_prio_not_ready)
        /**********************/
        {
            int prev_priority = -1;
            for (auto it = p->fired_actions_.begin(); it != p->fired_actions_.end();)
            /*******************************************************************/
            {
                if (it->second.priority < fired_priority && it->second.priority > prev_priority)
                /****************************************************************************/
                {
                    key = it->first;
                    fired_action = it->second.action;
                    prev_priority = ticksButtonDown + it->second.delay;
                    ticks_perform = ticksButtonDown + it->second.delay;
                    fired_priority = it->second.priority;
                }
                it++;
            }
        }
        Log();
        if (SDL_GetTicks() >= ticks_perform)
        /*********************************/
        {
            if (fired_action)
            /**************/
            {
                if (isPressed)
                    fired_action();
                p->fired_actions_.erase(key);
            }
        }
        Log();
        for (auto key : fired_to_delete)
            p->fired_actions_.erase(key);
        Log();
        if (p->fired_actions_.size() == 0 && isPressed == false)
            hasBeenFired = false;
    }

    void
    MouseActionManager::PerformReleasedActions(int ticksButtonDown)
    /*///////////////////////////////////////////////////////////*/
    {
        std::function<void()> released_action;
        float released_priority = -1;
        Uint64 ticks_perform = 0;
        int key = 0;
        bool highest_prio_not_ready = false;
        std::set<int> released_to_delete;
        Log();
        for (auto it = p->released_actions_.begin(); it != p->released_actions_.end();)
        /*************************************************************************/
        {
            Log();
            if (it->second.priority > released_priority)
            /*****************************************/
            {
                released_priority = it->second.priority;
                released_action = it->second.action;
                ticks_perform = ticksButtonDown + it->second.delay;
                key = it->first;
                auto diff = (int)(ticks_perform - SDL_GetTicks());
                Log();
                if (diff > 0)
                /***********/
                {
                    highest_prio_not_ready = true;
                    it++;
                    continue;
                }
            }
            Log();
            if (false == it->second.ensureIsExec)
                released_to_delete.insert(it->first);
            it++;
        }
        Log();
        if (highest_prio_not_ready)
        /**********************/
        {
            int prev_priority = -1;
            for (auto it = p->released_actions_.begin(); it != p->released_actions_.end();)
            /*************************************************************************/
            {
                if (it->second.priority < released_priority && it->second.priority > prev_priority)
                /*******************************************************************************/
                {
                    key = it->first;
                    released_action = it->second.action;
                    prev_priority = ticksButtonDown + it->second.delay;
                    ticks_perform = ticksButtonDown + it->second.delay;
                    released_priority = it->second.priority;
                }
                it++;
            }
        }
        Log();
        if (SDL_GetTicks() >= ticks_perform)
        /*********************************/
        {
            Log();
            if (released_action)
            /*****************/
            {
                Log();
                released_action();
                Log();
                p->released_actions_.erase(key);
            }
        }
        Log();
        for (auto key : released_to_delete)
            p->released_actions_.erase(key);
    }

    void
    MouseActionManager::ClearFiredActions()
    /*///////////////////////////////////*/
    {
        p->fired_actions_.clear();
    }

    void
    MouseActionManager::ClearReleasedActions()
    /*//////////////////////////////////////*/
    {
        p->released_actions_.clear();
    }

    bool
    MouseActionManager::FiredActionsContains(int actionId)
    /*//////////////////////////////////////////////////*/
    {
        return p->fired_actions_.count(actionId);
    }

    bool
    MouseActionManager::ReleasedActionsContains(int actionId)
    /*/////////////////////////////////////////////////////*/
    {
        return p->released_actions_.count(actionId);
    }

    void
    MouseActionManager::AddFiredAction(int actionId, MouseAction action)
    /*////////////////////////////////////////////////////////////////*/
    {
        p->fired_actions_.insert({actionId, action});
    }

    void
    MouseActionManager::AddReleasedAction(int actionId, MouseAction action)
    /*///////////////////////////////////////////////////////////////////*/
    {
        p->released_actions_.insert({actionId, action});
    }
}