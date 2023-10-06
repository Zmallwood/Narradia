//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Graphics : public Singleton<Graphics>
    /*///////////////////////////////////////*/
    {
      public:
        Graphics();
        SDL_Window *GetWindow() const;
        void ClearCanvas() const;
        void PresentCanvas() const;
        void Cleanup();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}