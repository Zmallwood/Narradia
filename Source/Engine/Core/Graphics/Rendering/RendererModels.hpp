//////////////////////////////////////////////////////////////////////
#pragma once
#include "RendererBase.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class RendererModels : public RendererBase,
                           public Singleton<RendererModels>
    /*///////////////////////////////////////////////////*/
    {
      public:
        RendererModels();
        void NewModel(int);
        void DrawModel(
            int, float, Point3F, float = 0.0f, float = 1.0f, float = 1.0f,
            glm::vec3 = {1.0f, 1.0f, 1.0f}, bool = false, bool = false) const;
        void DrawModelsMany(
            int, float, std::vector<Point3F>, std::vector<float>, std::vector<float>,
            std::vector<float>, std::vector<glm::vec3>) const;
        void StartBatchDrawing();
        void StopBatchDrawing();
        void Cleanup();

      private:
        RenderId NewBodyKeyframe(int, float, int);
        void
        NewBodyKeyframeGeometry(int, float, RenderId, std::vector<Vertex3F>, std::vector<Point3F>);

        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
} 