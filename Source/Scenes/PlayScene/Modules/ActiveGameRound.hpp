#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
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