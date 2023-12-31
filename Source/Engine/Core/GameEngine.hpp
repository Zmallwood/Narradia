#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GameEngine : public Singleton<GameEngine> {
      public:
        GameEngine();
        void Run() const;
        void StopGame();
        const bool IsRunning() const;

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}