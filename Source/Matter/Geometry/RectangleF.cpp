#include "RectangleF.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    RectangleF::RectangleF() = default;

    RectangleF::RectangleF(float x_, float y_, float width_, float height_)
        : x(x_),
          y(y_),
          width(width_),
          height(height_) {
    }

    RectangleF::RectangleF(Point2F position, SizeF size)
        : x(position.x),
          y(position.y),
          width(size.width),
          height(size.height) {
    }

    GlRectangleF RectangleF::ToGlRect() const {
        return {x * 2 - 1.0f, 1.0f - y * 2, width * 2, height * 2};
    }

    RectangleF RectangleF::InvertVertically() {
        return {x, -y, width, -height};
    }

    Point2F RectangleF::Center() const {
        return {x + width / 2, y + height / 2};
    }

    Point2F RectangleF::GetPosition() const {
        return {x, y};
    }

    SizeF RectangleF::GetSize() const {
        return {width, height};
    }

    bool RectangleF::Contains(Point2F point) const {
        return point.x >= x && point.y >= y && point.x < x + width && point.y < y + height;
    }

    RectangleF RectangleF::WithWidth(float otherWidth) const {
        return {x, y, otherWidth, height};
    }

    RectangleF RectangleF::Translate(float dx, float dy) const {
        return {x + dx, y + dy, width, height};
    }

    RectangleF RectangleF::MoveToOrigo() {
        return {0.0f, 0.0f, width, height};
    }
}