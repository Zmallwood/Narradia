#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class KeyboardMove
    /*//////////////*/
    {
      private:
        void Update();
        
        friend class PlayScene;
        friend class EditorScene;
    };
}