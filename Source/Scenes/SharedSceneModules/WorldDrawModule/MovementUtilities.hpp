#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Mob;
    class Companion;

    Point2F GetMinorMovementOffsetForMob(Mob *);
    Point2F GetMinorMovementOffsetForCompanion(Companion *);
}