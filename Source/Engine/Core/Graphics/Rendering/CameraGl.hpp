#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class CameraGl : public Singleton<CameraGl>
    /*///////////////////////////////////////*/
    {
      public:
        CameraGl();
        const glm::mat4 &GetPerspectiveMatrix();
        const glm::mat4 &GetViewMatrix();
        void SetPerspectiveMatrix(glm::mat4);
        void SetViewMatrix(glm::mat4);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}