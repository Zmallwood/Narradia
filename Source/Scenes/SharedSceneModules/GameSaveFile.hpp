#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GameSaveFile
    /*//////////////*/
    {
      public:
        void CreateRawData();
        void SaveCryptated();

      private:
        std::string_view rawData;
    };
}