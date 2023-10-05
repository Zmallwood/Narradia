//////////////////////////////////////////////////////////////////////
#include "ExperienceCalculations.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    int GetCurrentLevel(int exp)
    /*////////////////////////*/ {
        auto value = std::trunc(log2f(pow(exp + 70, 2) / 5000)) + 1;
        auto lvl = static_cast<int>(value);
        return lvl;
    }

    int GetExpForLevel(int lvl)
    /*///////////////////////*/ {
        auto value = std::max(sqrt(powf(2, lvl - 1) * 5000) - 70, 0.0f);
        auto exp = static_cast<int>(value);
        return exp;
    }

    int GetExpDiffToNextLevel(int exp)
    /*//////////////////////////////*/ {
        auto currLvl = GetCurrentLevel(exp);
        auto nextLvl = currLvl + 1;
        auto totExpCurrLvl = GetExpForLevel(currLvl);
        auto totExpNextLvl = GetExpForLevel(nextLvl);
        auto expDifference = totExpNextLvl - totExpCurrLvl;
        return expDifference;
    }

    int GetExpReqForCurrLevelStart(int exp)
    /*///////////////////////////////////*/ {
        auto currLvl = GetCurrentLevel(exp);
        auto totExpCurrLvlStart = GetExpForLevel(currLvl);
        return totExpCurrLvlStart;
    }

    int GetExpGainedSinceLevelStart(int exp)
    /*////////////////////////////////////*/ {
        auto expSinceStart = exp - GetExpReqForCurrLevelStart(exp);
        return expSinceStart;
    }

    float GetFractionalExpProgress(int exp)
    /*///////////////////////////////////*/ {
        auto expGain = exp - GetExpReqForCurrLevelStart(exp);
        auto expDifference = GetExpDiffToNextLevel(exp);
        auto progressFrac = static_cast<float>(expGain) / expDifference;
        return progressFrac;
    }
}
//////////////////////////////////////////////////////////////////////