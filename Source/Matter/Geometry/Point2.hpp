#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    struct Point2 {
        Point2 Translate(int, int) const;
        bool operator==(const Point2 &);
        int x = 0;
        int y = 0;
    };
}