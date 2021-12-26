#ifndef __RTC__CORE__SCENE_H__
#define __RTC__CORE__SCENE_H__

#include "configuration.h"
#include "camera.h"
#include "environment/sky.h"
#include "shape_node.h"

namespace ray_tracing_core
{
    namespace core
    {
        class Scene
        {
        private:

            const Configuration& configuration;
            const Camera& camera;
            const environment::Sky& sky;
            const ShapeNode& world;

        public:

            Scene(const Configuration& configuration, const Camera& camera, const environment::Sky& sky, const ShapeNode& world);

            Color ray_trace_color(double u, double v) const;
        };
    }
}

#endif
