//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MapFilesIO : public Singleton<MapFilesIO>
    /*///////////////////////////////////////////*/
    {
      public:
        /**
         * \brief Constructor.
         */
        MapFilesIO();
        /**
         * \brief Save current map area to file.
         * \param mapName Map name for the file to save to.
         */
        void SaveCurrentMapArea(std::string_view mapName);
        /**
         * \brief Load map area from file.
         * \param mapName Map name for file to load.
         */
        void LoadMapArea(std::string_view mapName);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}
//////////////////////////////////////////////////////////////////////