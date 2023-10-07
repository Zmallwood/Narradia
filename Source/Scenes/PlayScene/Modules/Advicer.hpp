#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Advicer : public Singleton<Advicer>
    /*/////////////////////////////////////*/
    {
      public:
        void Update();
        bool ready_for_next_advice_ = true;

      private:
        bool has_given_foraging_advice_ = false;
        bool has_given_create_stone_axe_advice_ = false;
    };
}