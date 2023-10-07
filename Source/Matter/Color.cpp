#include "Color.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    SDL_Color
    Color::ToSdlColor() const
    /*/////////////////////*/
    {
        return {
            static_cast<unsigned char>(r * 255), static_cast<unsigned char>(g * 255),
            static_cast<unsigned char>(b * 255), static_cast<unsigned char>(a * 255)};
    }
}