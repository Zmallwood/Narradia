#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    enum class ObjectBehaviourFlags : int
    /*/////////////////////////////////*/
    {
        MovementBlock = 1 << 0,
        Unmovable = 1 << 1,
        NoShadow = 1 << 2,
        MovementBlockN = 1 << 3,
        MovementBlockE = 1 << 4,
        MovementBlockS = 1 << 5,
        MovementBlockW = 1 << 6
    };
}