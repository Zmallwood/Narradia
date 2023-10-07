#include "CameraGl.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class CameraGl::Pimpl
    /*/////////////////*/
    {
      public:
        glm::mat4 perspective_matrix_;
        glm::mat4 view_matrix_;
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
        return p->perspective_matrix_;
    }

    const glm::mat4 &
    CameraGl::GetViewMatrix()
    /*/////////////////////*/
    {
        return p->view_matrix_;
    }

    void
    CameraGl::SetPerspectiveMatrix(glm::mat4 newPerspectiveMatrix)
    /*//////////////////////////////////////////////////////////*/
    {
        p->perspective_matrix_ = newPerspectiveMatrix;
    }

    void
    CameraGl::SetViewMatrix(glm::mat4 newViewMatrix)
    /*////////////////////////////////////////////*/
    {
        p->view_matrix_ = newViewMatrix;
    }
}