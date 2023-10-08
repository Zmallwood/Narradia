#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    struct ObjectBehaviourEntry {
        std::string_view label;
        int flags = 0;
        int container_slots = 0;
    };
}