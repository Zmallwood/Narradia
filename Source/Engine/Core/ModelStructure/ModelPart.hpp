#pragma once
#include "ModelPartTimeline.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ModelPart {
      public:
        ModelPart();
        int texNameHash = 0;
        std::unique_ptr<ModelPartTimeline> timeline;
    };
}