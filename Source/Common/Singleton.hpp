#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    template <class T> class Singleton {
      public:
        static void Create() {
            if (!instance)
                instance = std::make_shared<T>();
        }

        static std::shared_ptr<T> Get() {
            if (!instance)
                Create();
            return std::weak_ptr<T>(instance).lock();
        }

        static void Dispose() {
            instance.reset();
        }

      private:
        inline static std::shared_ptr<T> instance;
    };
}