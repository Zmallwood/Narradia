//////////////////////////////////////////////////////////////////////
#include "ModelPart.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    ModelPart::ModelPart()
        : timeline(std::make_unique<ModelPartTimeline>())
    /*/////////////////////////////////////////////////*/
    {
    }
}
//////////////////////////////////////////////////////////////////////