#include "Point2.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    Point2 Point2::Translate(int offsetX, int offsetY) const {
        return {x + offsetX, y + offsetY};
    }

    bool Point2::operator==(const Point2 &other) {
        return x == other.x && y == other.y;
    }
}