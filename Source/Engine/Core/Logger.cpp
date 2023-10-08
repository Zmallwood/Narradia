#include "Logger.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Logger::Pimpl {
      public:
        static constexpr std::string_view kLogDirectoryPath = "Logs";
        const bool kenabled = true;
        std::ofstream log_file_;
    };

    Logger::Logger()
        : p(std::make_shared<Pimpl>()) {
        std::filesystem::remove_all(
            std::string(SDL_GetBasePath()) + Pimpl::kLogDirectoryPath.data());
        std::filesystem::create_directory(
            std::string(SDL_GetBasePath()) + Pimpl::kLogDirectoryPath.data());
        auto log_file_name = "Log." + std::string(GetCurrentDateTime().data()) + ".txt";
        p->log_file_.open(
            std::string(SDL_GetBasePath()) + Pimpl::kLogDirectoryPath.data() + "/" + log_file_name,
            std::ofstream::out | std::ofstream::trunc);
    }

    Logger::~Logger() {
        p->log_file_.close();
    }

    void Logger::Log(std::string_view message, const std::source_location location) {
        if (!p->kenabled)
            return;
        auto std_str = std::string(location.file_name());
        auto file_name_start = std_str.find("Source/") + 7;
        auto file_name = std_str.substr(file_name_start);
        std::string log_text = std::string(GetCurrentTime().data()) + "." +
                               std::to_string(SDL_GetTicks() % 1000) + ")" + message.data() + "|" +
                               file_name + "(" + std::to_string(location.line()) + ")" +
                               "                                ";
        p->log_file_.seekp(0);
        p->log_file_ << log_text << std::flush;
    }

    void Log(const std::source_location location) {
        Logger::Get()->Log("", location);
    }
}