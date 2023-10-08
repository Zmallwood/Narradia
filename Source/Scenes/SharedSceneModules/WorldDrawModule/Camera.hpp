#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Camera : public Singleton<Camera> {
      public:
        Camera();
        void Update();
        float GetZoomAmount();
        Point3F MoveCloserToCamera(Point3F, float);

        float horizontal_angle_ = .0f;
        float vertical_angle_ = 45.f;
        Point3F camera_position_;
        float camera_distance_ = 150.f;

      private:
        void UpdateZooming();
        void CalculateCameraPosition();

        Point3F player_position_3d_;
        const float zoom_sensitivity_ = 0.15f;
        const float fov_ = 33.0f;
    };
}