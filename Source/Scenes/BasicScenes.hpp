//////////////////////////////////////////////////////////////////////
#pragma once
#include "Engine/Core/SceneBase.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class IntroScene : public SceneBase
    /*/////////////////////////////*/
    {
      public:
        IntroScene();

      private:
        void UpdateDerived() override;
        void RenderDerived() override;
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class MainMenuScene : public SceneBase
    /*////////////////////////////////*/
    {
      public:
        MainMenuScene();

      private:
        void UpdateDerived() override;
        void RenderDerived() override;
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class MapTypeSelectionScene : public SceneBase
    /*////////////////////////////////////////*/
    {
      public:
        MapTypeSelectionScene();

      private:
        void UpdateDerived() override;
        void RenderDerived() override;
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class UserMapSelectionScene : public SceneBase
    /*////////////////////////////////////////*/
    {
      public:
        UserMapSelectionScene();
        void UpdateDerived() override;
        void RenderDerived() override;

      private:
        RenderId glIdBackgroundImage;
    };
}
//////////////////////////////////////////////////////////////////////