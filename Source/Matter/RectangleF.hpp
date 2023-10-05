//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SizeF;
    
    class RectangleF
    /*////////////*/
    {
      public:
        RectangleF();
        RectangleF(float, float, float, float);
        RectangleF(Point2F, SizeF);
        GlRectangleF ToGlRect() const;
        RectangleF InvertVertically();
        Point2F Center() const;
        bool Contains(Point2F) const;
        Point2F GetPosition() const;
        SizeF GetSize() const;
        RectangleF Translate(float, float) const;
        RectangleF MoveToOrigo();
        RectangleF WithWidth(float) const;
        float x = 0.0f;
        float y = 0.0f;
        float width = 0.0f;
        float height = 0.0f;
    };
}
//////////////////////////////////////////////////////////////////////