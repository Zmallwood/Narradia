//////////////////////////////////////////////////////////////////////
#include "Trigonometry.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    float
    SinDegrees(float angleDeg)
    /*//////////////////////*/
    {
        return sinf(angleDeg * static_cast<float>(M_PI) / 180.0f);
    }

    float
    CosDegrees(float angleDeg)
    /*//////////////////////*/
    {
        return cosf(angleDeg * static_cast<float>(M_PI) / 180.0f);
    }
}