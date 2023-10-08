#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    struct MouseAction {
        std::function<void()> action;
        float priority = 0;
        Uint64 delay = 0;
        bool ensure_is_executed = false;
    };
}