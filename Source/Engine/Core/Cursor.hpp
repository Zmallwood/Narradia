#pragma once
#include "CursorTypes.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Cursor : public Singleton<Cursor>
    /*///////////////////////////////////*/
    {
      public:
        Cursor();
        void Render() const;
        void Reset();
        void SavePosition();
        void RestoreSavedPosition() const;
        Point2 GetSavedPosition() const;
        void SetCursorType(CursorTypes);
        void LockMousePos() const;
        void UnlockMousePos() const;
        void HideThisFrame();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}