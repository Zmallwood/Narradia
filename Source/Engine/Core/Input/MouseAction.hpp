#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MouseAction
    /*/////////////*/
    {
      public:
        std::function<void()> action;
        float priority = 0;
        Uint64 delay = 0;
        bool ensureIsExec = false;
    };
}