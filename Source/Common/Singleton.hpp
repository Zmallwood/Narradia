//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    typedef GLuint RenderId;

    class SceneBase;
    class SceneGui;

    template <class T>
    class Singleton
    /*//////////////////////////////*/
    {
      public:
        static void Create()
        /*////////////////*/
        {
            if (!instance)
                instance = std::make_shared<T>();
        }
        static std::shared_ptr<T> GetPointer()
        /*//////////////////////////////////*/
        {
            if (!instance)
                Create();
            return std::weak_ptr<T>(instance).lock();
        }
        static T &Get()
        /*///////////*/
        {
            if (!instance)
                Create();
            return *GetPointer();
        }
        static void Dispose()
        /*/////////////////*/
        {
            instance.reset();
        }
        inline static T &get = Singleton<T>::Get();

      private:
        inline static std::shared_ptr<T> instance;
    };
}
//////////////////////////////////////////////////////////////////////