//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Logger : public Singleton<Logger>
    /*///////////////////////////////////*/
    {
      public:
        Logger();
        ~Logger();
        void
            Log(std::string_view = "",
                const std::source_location = std::source_location::current());

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    void Log(const std::source_location location = std::source_location::current());
}