#include "Point2F.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    Point2F Point2F::Copy() {
        return *this;
    }

    Point2F Point2F::Translate(float offsetX, float offsetY) const {
        return {x + offsetX, y + offsetY};
    }

    Point2 Point2F::ToIntPoint() const {
        return {static_cast<int>(x), static_cast<int>(y)};
    }
}