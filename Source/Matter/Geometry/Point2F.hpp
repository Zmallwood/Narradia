#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Point2F
    /*/////////*/
    {
      public:
        Point2F Copy();
        Point2F Translate(float, float) const;
        Point2 ToIntPoint() const;
        float x = 0.0f;
        float y = 0.0f;
    };
}