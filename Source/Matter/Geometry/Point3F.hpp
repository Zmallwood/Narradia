#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    struct Point3F {
        Point3F Translate(float, float, float);
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    };
}