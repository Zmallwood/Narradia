//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class DestinationMove : public Singleton<DestinationMove>
    /*/////////////////////////////////////////////////////*/
    {
      public:
        void ResetDestination();
        const Point2 &GetDestination();

      private:
        void Update();
        Point2 destination = {-1, -1};
        friend class PlayScene;
    };
}