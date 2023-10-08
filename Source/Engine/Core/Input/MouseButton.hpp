#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MouseButton {
      public:
        MouseButton();
        void Reset();
        void PressDown();
        void Release();
        int GetPressDuration() const;
        int ClickDurationPickResult();
        int ClickDurationPeekResult() const;
        void
        AddFiredAction(std::string_view, std::function<void()>, float = 0, int = 0, bool = false);
        void AddReleasedAction(
            std::string_view, std::function<void()>, float = 0, int = 0, bool = false);
        void PerformMouseAction();
        void ResetActions();
        static const int GetDefaultClickSpeed();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}