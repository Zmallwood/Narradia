#include "Point3F.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    Point3F Point3F::Translate(float offsetX, float offsetY, float offsetZ) {
        return {x + offsetX, y + offsetY, z + offsetZ};
    }
}