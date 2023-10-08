#include "Colors.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    namespace
    {
        constexpr auto _0 = .0f;
        constexpr auto _5 = .05f;
        constexpr auto _10 = .1f;
        constexpr auto _15 = .15f;
        constexpr auto _20 = .2f;
        constexpr auto _25 = .25f;
        constexpr auto _30 = .3f;
        constexpr auto _40 = .4f;
        constexpr auto _50 = .5f;
        constexpr auto _60 = .6f;
        constexpr auto _70 = .7f;
        constexpr auto _80 = .8f;
        constexpr auto _90 = .9f;
        constexpr auto _100 = 1.f;
    }

    namespace Colors
    {
        const Color alphaYellow = {_100, _100, _0, _70};
        const Color black = {_0, _0, _0, _100};
        const Color white = {_100, _100, _100, _100};
        const Color elegantBlue = {_0, _15, _25, _100};
        const Color alphaElegantBlue = {_0, _15, _25, _60};
        const Color mildBlue = {_0, _50, _100, _100};
        const Color alphaMildBlue = {_0, _50, _100, _30};
        const Color wheat = {_100, _100, _70, _100};
        const Color alphaWheat = {_100, _100, _50, _70};
        const Color purple = {_50, _0, _100, _100};
        const Color green = {_0, _70, _0, _100};
        const Color darkGreen = {_0, _20, _0, _100};
        const Color yellow = {_100, _100, _0, _100};
        const Color red = {_100, _0, _0, _100};
        const Color darkPurple = {_20, _0, _20, _100};
        const Color darkRed = {_50, _0, _0, _100};
        const Color darkYellow = {_50, _50, _0, _100};
        const Color gray = {_50, _50, _50, _100};
        const Color darkGray = {_25, _25, _25, _100};
        const Color alphaWhite = {_100, _100, _100, _70};
        const Color alphaBlack = {_0, _0, _0, _70};
    }
}