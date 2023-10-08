#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ActionRepeat : public Singleton<ActionRepeat> {
      public:
        ActionRepeat();
        void RepeatLastAction();

      private:
        void Update();
        void SetRepeatingAction(std::function<void()>);

        class Pimpl;
        std::shared_ptr<Pimpl> p;
        friend class PlayScene;
        friend class SplitStackGui;
        friend class InteractionMenu;
    };
}