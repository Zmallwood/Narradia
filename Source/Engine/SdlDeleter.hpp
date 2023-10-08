#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SdlDeleter {
      public:
        void operator()(SDL_Window *) const;
        void operator()(SDL_Renderer *) const;
        void operator()(SDL_Surface *) const;
        void operator()(SDL_Texture *) const;
        void operator()(TTF_Font *) const;
    };
}