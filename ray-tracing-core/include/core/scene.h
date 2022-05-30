#ifndef __RTC__CORE__SCENE_H__
#define __RTC__CORE__SCENE_H__

#include "configuration.h"
#include "camera.h"
#include "environment/sky.h"
#include "shape_node.h"
#include "geometry/geometry.h"

namespace ray_tracing_core
{
    namespace core
    {
        class Scene
        {
        protected:
            Configuration configuration;
            const Camera* camera = nullptr;
            const environment::Sky* sky = nullptr;
            const ShapeNode* world = nullptr;
            std::vector<const geometry::Geometry*> lights;

        public:
            Scene() = default;
            Scene(const Scene&) = default;
            Scene(const Configuration& configuration, const Camera& camera, const environment::Sky& sky, const ShapeNode& world);
            Scene& operator=(const Scene&) = default;
            Color ray_trace_color(double u, double v) const;
            void set_configuration(const Configuration &new_configuration) { configuration = new_configuration; }
            void set_camera(const Camera* new_camera) { camera = new_camera; }
            void set_sky(const environment::Sky* new_sky) { sky = new_sky; }
            void set_world(const ShapeNode* new_world) { world = new_world; }
            void set_lights(const std::vector<const geometry::Geometry*> &new_lights) { lights = new_lights; }
            const Configuration& get_configuration() const { return configuration; }
            const Camera* get_camera() const { return camera; }
            const environment::Sky* get_sky() const { return sky; }
            const ShapeNode* get_world() const { return world; }
            const std::vector<const geometry::Geometry*> get_lights() { return lights; }
        };
    }
}

#endif
