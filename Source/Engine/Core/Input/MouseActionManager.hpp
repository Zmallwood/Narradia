#pragma once
#include "MouseAction.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MouseActionManager {
      public:
        MouseActionManager();
        void PerformFiredActions(int, bool, bool &);
        void PerformReleasedActions(int);
        bool FiredActionsContains(int);
        bool ReleasedActionsContains(int);
        void ClearFiredActions();
        void ClearReleasedActions();
        void AddFiredAction(int, MouseAction);
        void AddReleasedAction(int, MouseAction);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}