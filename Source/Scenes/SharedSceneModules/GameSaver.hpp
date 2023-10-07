#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GameSaver : public Singleton<GameSaver>
    /*/////////////////////////////////////////*/
    {
      public:
        void LoadSavedGame(std::string_view);
        void SaveGame(std::string_view);
    };
}