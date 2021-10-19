#include "shape_list_test.h"
#include <core/shape_list.h>
#include <core/shape.h>
#include <geometry/sphere.h>
#include <material/no_material.h>
#include <rtc_core_test_assert.h>

namespace ray_tracing_core_unit_test
{
    namespace core
    {
        using namespace ray_tracing_core;
        using namespace ray_tracing_core::core;

        void shape_list_bounding_box_test()
        {
            std::vector<std::tuple<math::Point3D, math::Point3D, geometry::Sphere, geometry::Sphere>> test_data
            {
                { math::Point3D(-2, -1, -1), math::Point3D(2, 1, 1),
                  geometry::Sphere(math::Point3D(-1, 0, 0), 1), geometry::Sphere(math::Point3D(1, 0, 0), 1) }
            };

            for (auto [expected_minimum, expected_maximum, sphere1, sphere2] : test_data)
            {
                material::NoMaterial no_material;
                Shape shape1(&sphere1, &no_material);
                Shape shape2(&sphere2, &no_material);
                auto actual_box = ShapeList(std::vector<const ShapeNode*>{ &shape1, & shape2})
                    .bounding_box();
                assert_equal_vector(expected_minimum, actual_box.minimum_point, 0.001);
                assert_equal_vector(expected_maximum, actual_box.maximum_point, 0.001);
            }
        }
    }
}