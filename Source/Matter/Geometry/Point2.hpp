#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Point2
    /*////////*/
    {
      public:
        Point2 Translate(int, int) const;
        bool operator==(const Point2 &);
        int x = 0;
        int y = 0;
    };
}