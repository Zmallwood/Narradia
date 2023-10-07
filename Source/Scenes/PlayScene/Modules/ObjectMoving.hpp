#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Object;
    
    class ObjectMoving : public Singleton<ObjectMoving>
    /*///////////////////////////////////////////////*/
    {
      public:
        ObjectMoving();
        std::shared_ptr<Object> objectInAir = nullptr;

      private:
        void Update();
        void Render();
        void PickupObjectFromGuiIfIsTheCase();
        void PickupObjectFromGroundIfIsTheCase();
        void ReleaseObjectIfIsTheCase();
        RenderId id_moving_object_image;
        const float k_object_image_size = 0.03f;
        friend class PlayScene;
    };
}