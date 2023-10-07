#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    enum class Tools
    /*////////////*/
    {
        None,
        SetRoof,
        AddObject,
        AddMob,
        SetGround,
        AlterElevation
    };
    
    class ToolUsing : public Singleton<ToolUsing>
    /*/////////////////////////////////////*/
    {
      public:
        void Update();
        void ChangeTool(Tools newTool);
        void SelectType(int newSelectedObjectType);
        void IncreaseToolRadius();
        void DecreaseToolRadius();
        void IncreaseElevationChange();
        void DecreaseElevationChange();

      private:
        Tools current_tool_ = Tools::None;
        int selected_type_ = 0;
        int tool_radius_ = 1;
        int elevation_change_ = 1;
    };
}