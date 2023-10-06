//////////////////////////////////////////////////////////////////////
#include "Logger.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Logger::Pimpl
    /*///////////////*/
    {
      public:
        static constexpr std::string_view logDirPath = "Logs";
        std::ofstream logFile;
        bool enabled = true;
    };

    Logger::Logger()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
        std::filesystem::remove_all(std::string(SDL_GetBasePath()) + Pimpl::logDirPath.data());
        std::filesystem::create_directory(
            std::string(SDL_GetBasePath()) + Pimpl::logDirPath.data());
        auto logFileName = "Log." + std::string(GetCurrentDateTime().data()) + ".txt";
        p->logFile.open(
            std::string(SDL_GetBasePath()) + Pimpl::logDirPath.data() + "/" + logFileName,
            std::ofstream::out | std::ofstream::trunc);
    }

    Logger::~Logger()
    /*/////////////*/
    {
        p->logFile.close();
    }

    void Logger::Log(std::string_view message, const std::source_location location)
    /*///////////////////////////////////////////////////////////////////////////*/
    {
        if (!p->enabled)
            return;
        auto stdstr = std::string(location.file_name());
        auto fileNameStart = stdstr.find("Source/") + 7;
        auto fileName = stdstr.substr(fileNameStart);
        std::string logText = std::string(GetCurrentTime().data()) + "." +
                              std::to_string(SDL_GetTicks() % 1000) + ")" + message.data() + "|" +
                              fileName + "(" + std::to_string(location.line()) + ")" +
                              "                                ";
        p->logFile.seekp(0);
        p->logFile << logText << std::flush;
    }

    void Log(const std::source_location location)
    /*//////*/
    {
        Logger::Get().Log("", location);
    }
}
//////////////////////////////////////////////////////////////////////