#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ObjectBehaviourEntry
    /*//////////////////////*/
    {
      public:
        std::string_view label;
        int flags = 0;
        int containerSlots = 0;
    };
}