//////////////////////////////////////////////////////////////////////
#pragma once
#include "ObjectBehaviourFlags.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ObjectBehaviourList : public Singleton<ObjectBehaviourList>
    /*/////////////////////////////////////////////////////////////*/ {
      public:
        ObjectBehaviourList();
        void Initialize();
        void AddLabel(std::string_view, std::string_view);
        void AddFlag(std::string_view, ObjectBehaviourFlags);
        void SetContainerSlots(std::string_view, int);
        bool IsContainer(int);
        bool HasBehaviourData(int);
        int GetFlags(int);
        std::string_view GetLabel(int);
        int GetContainerSlots(int);
        void Clear();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}
//////////////////////////////////////////////////////////////////////