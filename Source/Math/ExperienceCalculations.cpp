#include "ExperienceCalculations.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    int
    GetCurrentLevel(int exp)
    /*////////////////////*/
    {
        auto value = std::trunc(log2f(pow(exp + 70, 2) / 5000)) + 1;
        auto level = static_cast<int>(value);
        return level;
    }

    int
    GetExpForLevel(int lvl)
    /*///////////////////*/
    {
        auto value = std::max(sqrt(powf(2, lvl - 1) * 5000) - 70, 0.0f);
        auto experience = static_cast<int>(value);
        return experience;
    }

    int
    GetExpDiffToNextLevel(int exp)
    /*//////////////////////////*/
    {
        auto current_level = GetCurrentLevel(exp);
        auto next_level = current_level + 1;
        auto total_experience_current_level = GetExpForLevel(current_level);
        auto total_experience_next_level = GetExpForLevel(next_level);
        auto experience_difference = total_experience_next_level - total_experience_current_level;
        return experience_difference;
    }

    int
    GetExpReqForCurrLevelStart(int exp)
    /*///////////////////////////////*/
    {
        auto current_level = GetCurrentLevel(exp);
        auto total_experience_current_level_start = GetExpForLevel(current_level);
        return total_experience_current_level_start;
    }

    int
    GetExpGainedSinceLevelStart(int exp)
    /*////////////////////////////////*/
    {
        auto experience_since_start = exp - GetExpReqForCurrLevelStart(exp);
        return experience_since_start;
    }

    float
    GetFractionalExpProgress(int exp)
    /*/////////////////////////////*/
    {
        auto experience_gain = exp - GetExpReqForCurrLevelStart(exp);
        auto experience_difference = GetExpDiffToNextLevel(exp);
        auto progress_fractional = static_cast<float>(experience_gain) / experience_difference;
        return progress_fractional;
    }
}