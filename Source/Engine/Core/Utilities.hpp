//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Size;
    class Point2;
    class Point2F;

    Size GetCanvasSize();
    float GetAspectRatio();
    float ConvertWidthToHeight(float);
    std::string_view GetCurrentTime();
    std::string_view GetCurrentDateTime();
    const int Hash(const std::string_view &);
    Point2 GetMousePositionPx();
    Point2F GetMousePositionF();
}