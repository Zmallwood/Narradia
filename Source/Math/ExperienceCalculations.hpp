#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    int GetCurrentLevel(int);
    int GetExpForLevel(int);
    int GetExpDiffToNextLevel(int);
    int GetExpReqForCurrLevelStart(int);
    int GetExpGainedSinceLevelStart(int);
    float GetFractionalExpProgress(int);
}