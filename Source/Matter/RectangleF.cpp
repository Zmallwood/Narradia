//////////////////////////////////////////////////////////////////////
#include "RectangleF.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    RectangleF::RectangleF() = default;

    RectangleF::RectangleF(float x_, float y_, float width_, float height_)
        : x(x_),
          y(y_),
          width(width_),
          height(height_)
    /*///////////////////////////////////////////////////////////////////*/ {} // Function

    RectangleF::RectangleF(Point2F position, SizeF size)
        : x(position.x),
          y(position.y),
          width(size.width),
          height(size.height)
    /*////////////////////////////////////////////////*/ {} // Function

    GlRectangleF RectangleF::ToGlRect() const
    /*/////////////////////////////////////*/ {
        return {x * 2 - 1.0f, 1.0f - y * 2, width * 2, height * 2};
    } // Function

    RectangleF RectangleF::InvertVertically()
    /*/////////////////////////////////////*/ { return {x, -y, width, -height}; } // Function

    Point2F RectangleF::Center() const
    /*//////////////////////////////*/ { return {x + width / 2, y + height / 2}; } // Function

    Point2F RectangleF::GetPosition() const
    /*///////////////////////////////////*/ { return {x, y}; } // Function

    SizeF RectangleF::GetSize() const
    /*/////////////////////////////*/ { return {width, height}; } // Function

    bool RectangleF::Contains(Point2F point) const
    /*//////////////////////////////////////////*/ {
        return point.x >= x && point.y >= y && point.x < x + width && point.y < y + height;
    } // Function

    RectangleF RectangleF::WithWidth(float otherWidth) const
    /*////////////////////////////////////////////////////*/ {
        return {x, y, otherWidth, height};
    } // Function

    RectangleF RectangleF::Translate(float dx, float dy) const
    /*//////////////////////////////////////////////////////*/ {
        return {x + dx, y + dy, width, height};
    } // Function

    RectangleF RectangleF::MoveToOrigo()
    /*////////////////////////////////*/ { return {0.0f, 0.0f, width, height}; } // Function
}
//////////////////////////////////////////////////////////////////////