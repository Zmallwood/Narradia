#include "Utilities.hpp"
#include "Graphics/Graphics.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    Size
    GetCanvasSize()
    /*///////////*/
    {
        int width;
        int height;
        SDL_GetWindowSize(Graphics::Get()->GetWindow(), &width, &height);
        return {width, height};
    }

    float
    GetAspectRatio()
    /*////////////*/
    {
        auto canvasSize = GetCanvasSize();
        return static_cast<float>(canvasSize.width) / canvasSize.height;
    }

    float
    ConvertWidthToHeight(float width)
    /*/////////////////////////////*/
    {
        return width * GetAspectRatio();
    }

    std::string_view
    GetCurrentTime()
    /*////////////*/
    {
        time_t now = time(0);
        char buffer[80];
        auto pTstruct = localtime(&now);
        strftime(buffer, sizeof(buffer), "%X", pTstruct);
        return buffer;
    }

    std::string_view
    GetCurrentDateTime()
    /*////////////////*/
    {
        time_t now = time(0);
        char buffer[80];
        auto pTstruct = localtime(&now);
        strftime(buffer, sizeof(buffer), "%Y.%m.%d.%H.%M.%S", pTstruct);
        return buffer;
    }

    const int
    Hash(const std::string_view &text)
    /*//////////////////////////////*/
    {
        return std::hash<std::string_view>{}(text);
    }

    Point2
    GetMousePositionPx()
    /*////////////////*/
    {
        int x;
        int y;
        SDL_GetMouseState(&x, &y);
        return {x, y};
    }

    Point2F
    GetMousePositionF()
    /*///////////////*/
    {
        auto canvasSize = GetCanvasSize();
        auto mousePosPx = GetMousePositionPx();
        auto x = static_cast<float>(mousePosPx.x) / canvasSize.width;
        auto y = static_cast<float>(mousePosPx.y) / canvasSize.height;
        return {x, y};
    }
}