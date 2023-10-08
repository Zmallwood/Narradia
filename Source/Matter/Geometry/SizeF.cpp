#include "SizeF.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    RectangleF SizeF::ToRectangle() const {
        return {0.0f, 0.0f, width, height};
    }
}