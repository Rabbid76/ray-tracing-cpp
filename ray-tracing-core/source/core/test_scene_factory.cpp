#include <core/camera.h>
#include <core/configuration.h>
#include <core/scene.h>
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
        texture::ConstantTexture sphere_texture;
        texture::ConstantTexture ground_texture;
        material::LambertianMaterial sphere_material;
        material::LambertianMaterial ground_material;
        geometry::Sphere sphere_geometry;
        geometry::Sphere ground_geometry;
        core::Shape sphere_shape;
        core::Shape ground_shape;
        core::ShapeList world;
        core::Camera camera;
        environment::Sky sky;
    
    public:
        TestScene(const Configuration &configuration, double aspect)
            : sphere_texture(math::ColorRGB(0.5f, 0.1f, 0.1f), 1)
            , ground_texture(math::ColorRGB(0.1f, 0.1f, 0.1f), 1)
            , sphere_material(&sphere_texture)
            , ground_material(&ground_texture)
            , sphere_geometry(math::Point3D(0, 0, 0), 0.5)
            , ground_geometry(math::Point3D(0, -1000.5, 0), 1000.0)
            , sphere_shape(&sphere_geometry, &sphere_material)
            , ground_shape(&ground_geometry, &ground_material)
            , world({ &sphere_shape, &ground_shape })
            , camera(core::Camera::new_camera_from_look_at(
                math::Point3D(0, 0, 1),
                math::Point3D(0),
                math::Vector3D(0, 1, 0),
                90, aspect, 0, 1, math::TimeRange{ 0, 0 }))
            , sky(environment::Sky::new_sky(math::ColorRGB(1, 1, 1), math::ColorRGB(0.5, 0.7, 1)))
            , core::Scene(configuration, camera, sky, world)
        {

        }
    };
}

Scene* TestSceneFactory::new_scene() const
{
    return new TestScene(configuration, aspect);
}
