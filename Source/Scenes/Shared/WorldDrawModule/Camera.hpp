//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Camera : public Singleton<Camera>
    /*///////////////////////////////////*/ {
      public:
        Camera();
        void Update();
        float GetZoomAmount();
        Point3F MoveCloserToCamera(Point3F, float);
        float horizontalAngle = .0f;
        float verticalAngle = 45.f;
        Point3F cameraPosition;
        float cameraDistance = 150.f;

      private:
        void UpdateZooming();
        void CalculateCameraPosition();
        Point3F playerPosition3D;
        const float zoomSens = 0.15f;
        const float fov = 33.0f;
    };
}
//////////////////////////////////////////////////////////////////////