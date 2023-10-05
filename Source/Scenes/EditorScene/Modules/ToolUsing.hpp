//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    enum class Tools
    /*////////////*/ {
        None,
        SetRoof,
        AddObject,
        AddMob,
        SetGround,
        AlterElevation
    };
    
    class ToolUsing : public Singleton<ToolUsing>
    /*/////////////////////////////////////*/ {
      public:
        void Update();
        void ChangeTool(Tools newTool);
        void SelectType(int newSelectedObjectType);
        void IncreaseToolRadius();
        void DecreaseToolRadius();
        void IncreaseElevationChange();
        void DecreaseElevationChange();

      private:
        Tools currentTool = Tools::None;
        int selectedType = 0;
        int toolRadius = 1;
        int elevationChange = 1;
    };
}
//////////////////////////////////////////////////////////////////////