//////////////////////////////////////////////////////////////////////
#pragma once
#include "Matter/Size.hpp"
#include "Matter/Point2.hpp"
#include "Matter/Point2F.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    Size GetCanvasSize();
    float GetAspectRatio();
    float ConvertWidthToHeight(float);
    std::string_view GetCurrentTime();
    std::string_view GetCurrentDateTime();
    const int Hash(const std::string_view &);
    Point2 GetMousePositionPx();
    Point2F GetMousePositionF();
}
//////////////////////////////////////////////////////////////////////