#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    struct Color {
        SDL_Color ToSdlColor() const;
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        float a = 0.0f;
    };
}