#include <core/camera.h>
#include <core/configuration.h>
#include <core/scene.h>
#include <core/scene_objects_container.h> 
#include <core/shape.h>
#include <core/shape_list.h>
#include <core/test_scene_factory.h>
#include <environment/sky.h>
#include <geometry/sphere.h>
#include <material/lambertian_material.h>
#include <texture/constant_texture.h>

using namespace ray_tracing_core;
using namespace ray_tracing_core::core;

namespace
{
    class TestScene
        : public Scene
    {
    private:
        core::SceneObjectContainer scene_objects;
    
    public:
        TestScene(const Configuration &configuration, double aspect)
        {
            scene_objects.textures.emplace_back(new texture::ConstantTexture(math::ColorRGB(0.5f, 0.1f, 0.1f), 1));
            scene_objects.textures.emplace_back(new texture::ConstantTexture(math::ColorRGB(0.1f, 0.1f, 0.1f), 1));
            scene_objects.materials.emplace_back(new material::LambertianMaterial(scene_objects.textures[0].get()));
            scene_objects.materials.emplace_back(new material::LambertianMaterial(scene_objects.textures[1].get()));
            scene_objects.geometries.emplace_back(new geometry::Sphere(math::Point3D(0, 0, 0), 0.5));
            scene_objects.geometries.emplace_back(new geometry::Sphere(math::Point3D(0, -1000.5, 0), 1000.0));
            scene_objects.shape_nodes.emplace_back(new core::Shape(scene_objects.geometries[0].get(), scene_objects.materials[0].get()));
            scene_objects.shape_nodes.emplace_back(new core::Shape(scene_objects.geometries[1].get(), scene_objects.materials[1].get()));
            scene_objects.shape_nodes.emplace_back(new core::ShapeList({ scene_objects.shape_nodes [0].get(), scene_objects.shape_nodes[1].get() }));
            scene_objects.skys.emplace_back(new environment::Sky(environment::Sky::new_sky(math::ColorRGB(1, 1, 1), math::ColorRGB(0.5, 0.7, 1))));
            scene_objects.cameras.emplace_back(new core::Camera(core::Camera::new_camera_from_look_at(
                math::Point3D(0, 0, 1),
                math::Point3D(0),
                math::Vector3D(0, 1, 0),
                90, aspect, 0, 1, math::TimeRange{ 0, 0 })));
            set_configuration(configuration);
            set_camera(scene_objects.cameras[0].get());
            set_sky(scene_objects.skys[0].get());
            set_world(scene_objects.shape_nodes[2].get());
        }
    };
}

Scene* TestSceneFactory::new_scene(const std::string& argument) const
{
    return new TestScene(configuration, aspect);
}
