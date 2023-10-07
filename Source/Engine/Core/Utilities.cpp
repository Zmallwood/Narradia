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
        auto canvas_size = GetCanvasSize();
        return static_cast<float>(canvas_size.width) / canvas_size.height;
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
        auto p_tstruct = localtime(&now);
        strftime(buffer, sizeof(buffer), "%X", p_tstruct);
        return buffer;
    }

    std::string_view
    GetCurrentDateTime()
    /*////////////////*/
    {
        time_t now = time(0);
        char buffer[80];
        auto p_tstruct = localtime(&now);
        strftime(buffer, sizeof(buffer), "%Y.%m.%d.%H.%M.%S", p_tstruct);
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
        auto canvas_size = GetCanvasSize();
        auto mouse_position_pixels = GetMousePositionPx();
        auto x = static_cast<float>(mouse_position_pixels.x) / canvas_size.width;
        auto y = static_cast<float>(mouse_position_pixels.y) / canvas_size.height;
        return {x, y};
    }
}