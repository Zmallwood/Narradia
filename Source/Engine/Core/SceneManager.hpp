//////////////////////////////////////////////////////////////////////
#pragma once
#include "SceneNames.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SceneManager : public Singleton<SceneManager>
    /*/////////////////////////////////////////////*/
    {
      public:
        SceneManager();
        void InitializeScenes();
        void UpdateCurrentScene();
        void RenderCurrentScene() const;
        void FinalizeCurrentScene();
        void ChangeScene(SceneNames);
        SceneNames GetCurrentScene();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}
