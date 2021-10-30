#include "ray_test.h"
#include <math/ray.h>
#include <rtc_test_assert.h>
#include <tuple>
#include <vector>

namespace ray_tracing_core_unit_test
{
    namespace math
    {
        using namespace ray_tracing_core::math;

        void ray_point_at_test(void)
        {
            std::vector<std::tuple<Point3D, Point3D, Vector3D, Distance>> test_data =
            {
                { Point3D(9, 12, 15), Point3D(1, 2, 3), Vector3D(4, 5, 6), 2},
                { Point3D(1, 2, 3), Point3D(1, 2, 3), Vector3D(0, 0, 0), 2},
                { Point3D(1, 2, 3), Point3D(1, 2, 3), Vector3D(4, 5, 6), 0},
                { Point3D(8, 10, 12), Point3D(0, 0, 0), Vector3D(4, 5, 6), 2},
                { Point3D(0, 0, 0), Point3D(0, 0, 0), Vector3D(0, 0, 0), 2},
                { Point3D(0, 0, 0), Point3D(0, 0, 0), Vector3D(4, 5, 6), 0}
            };

            for (auto [expected_point, origin, direction, distance] : test_data)
            {
                auto actual_point = Ray::new_ray(origin, direction)
                    .point_at(distance);
                assert_equal_point(expected_point, actual_point, 0.001);
            }
        }

        void ray_with_attributes_test(void)
        {
            std::vector<std::tuple<Point3D, Vector3D, Time>> test_data
            {
                { Point3D(1, 2, 3), Vector3D(4, 5, 6), 7 },
            };

            for (auto [origin, direction, time] : test_data)
            {
                Ray attributes{ .time = time };
                auto actual_ray = Ray::new_ray_with_attributes(origin, direction, attributes);
                assert_equal_point(origin, actual_ray.origin, 0);
                assert_equal_vector(direction, actual_ray.direction, 0);
                TEST_ASSERT_EQUAL(time, actual_ray.time);
            }
        }
    }
}
