//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class PlaySceneHotkeys
    /*/////////////////*/
    {
      private:
        static void Update();
        friend class PlayScene;
    };

    class ActionRepeat : public Singleton<ActionRepeat>
    /*///////////////////////////////////////////////*/
    {
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
    
    class ActiveGameRound : public Singleton<ActiveGameRound>
    /*/////////////////////////////////////////////////////*/
    {
      public:
        ActiveGameRound();

      private:
        void Update();
        void Render();
        bool GameIsCompleted();
        class Pimpl;
        std::shared_ptr<Pimpl> p;
        friend class PlayScene;
    };
}
//////////////////////////////////////////////////////////////////////