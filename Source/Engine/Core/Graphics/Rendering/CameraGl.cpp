#include "CameraGl.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class CameraGl::Pimpl
    /*/////////////////*/
    {
      public:
        glm::mat4 perspectiveMatrix;
        glm::mat4 viewMatrix;
    };

    CameraGl::CameraGl()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    const glm::mat4 &
    CameraGl::GetPerspectiveMatrix()
    /*////////////////////////////*/
    {
        return p->perspectiveMatrix;
    }

    const glm::mat4 &
    CameraGl::GetViewMatrix()
    /*/////////////////////*/
    {
        return p->viewMatrix;
    }

    void
    CameraGl::SetPerspectiveMatrix(glm::mat4 newPerspectiveMatrix)
    /*//////////////////////////////////////////////////////////*/
    {
        p->perspectiveMatrix = newPerspectiveMatrix;
    }

    void
    CameraGl::SetViewMatrix(glm::mat4 newViewMatrix)
    /*////////////////////////////////////////////*/
    {
        p->viewMatrix = newViewMatrix;
    }
}