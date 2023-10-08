#pragma once
#include "Engine/Core/ModelStructure/ModelPart.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Model {
      public:
        Model(int);
        const int animDuration = 0;
        std::vector<std::shared_ptr<const ModelPart>> modelParts;
    };
}