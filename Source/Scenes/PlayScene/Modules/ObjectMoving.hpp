#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Object;

    class ObjectMoving : public Singleton<ObjectMoving> {
      public:
        ObjectMoving();

        std::shared_ptr<Object> object_in_air_ = nullptr;

      private:
        void Update();
        void Render();
        void PickupObjectFromGuiIfIsTheCase();
        void PickupObjectFromGroundIfIsTheCase();
        void ReleaseObjectIfIsTheCase();

        const float kObjectImageSize = 0.03f;
        RenderId rendid_moving_object_image_;
        friend class PlayScene;
    };
}