#include "sphere_test.h"
#include <math/sphere.h>
#include <rtc_core_test_assert.h>
#include <tuple>
#include <vector>

namespace ray_tracing_core_unit_test
{
    namespace math
    {
        using namespace ray_tracing_core::math;

        void sphere_bounding_box_test(void)
        {
            std::vector<std::tuple<Point3D, Point3D, Point3D, Distance>> test_data
            {
                {Point3D(-3, -2, -1), Point3D(5, 6, 7), Point3D(1, 2, 3), 4},
            };

            for (auto [expected_minimum, expected_maximum, center, radius] : test_data)
            {
                auto actual_box = Sphere(center, radius)
                    .bounding_box();
                assert_equal_point(expected_minimum, actual_box.minimum_point, 0.001);
                assert_equal_point(expected_maximum, actual_box.maximum_point, 0.001);
            }
        }

        void sphere_hit_test(void)
        {
            std::vector<std::tuple<bool, HitPoint, Sphere, Ray, DistanceRange>> test_data
            {
                {
                    false, HitPoint::zero(),
                    Sphere::new_sphere(Point3D(0), 1), Ray::new_ray(Point3D(-2, 0, 0), Vector3D(-1, 0, 0)), { 0, 4 }
                },
                {
                    true, HitPoint::new_hit(1, Point3D(-1, 0, 0), Vector3D(-1, 0, 0)),
                    Sphere::new_sphere(Point3D(0), 1), Ray::new_ray(Point3D(-2, 0, 0), Vector3D(1, 0, 0)), { 0, 4 }
                },
                {
                    false, HitPoint::zero(),
                    Sphere::new_sphere(Point3D(0), 1), Ray::new_ray(Point3D(-2, 0, 0), Vector3D(1, 0, 0)), { 0, 0.5 }
                },
                {
                    false, HitPoint::zero(),
                    Sphere::new_sphere(Point3D(0), 1), Ray::new_ray(Point3D(-2, 0, 0), Vector3D(1, 0, 0)), { 3.5, 4 }
                },
                {
                    true, HitPoint::new_hit(3, Point3D(1, 0, 0), Vector3D(1, 0, 0)),
                    Sphere::new_sphere(Point3D(0), 1), Ray::new_ray(Point3D(-2, 0, 0), Vector3D(1, 0, 0)), { 2, 4 }
                },
                {
                    false, HitPoint::zero(),
                    Sphere::new_sphere(Point3D(0), 1), Ray::new_ray(Point3D(-2, 1, 0), Vector3D(1, 0, 0)), { 0, 4 }
                },
            };

            for (auto [expected_result, expected_hit_point, sphere, ray, distance_range] : test_data)
            {
                auto actual_hit_point = HitPoint::zero();
                auto actual_result = sphere.hit(ray, distance_range, actual_hit_point);
                TEST_ASSERT_EQUAL(expected_result, actual_result);
                if (expected_result && actual_result)
                {
                    TEST_ASSERT_EQUAL_DELTA(expected_hit_point.distance, actual_hit_point.distance, 0.001);
                    assert_equal_point(expected_hit_point.position, actual_hit_point.position, 0.001);
                    assert_equal_vector(expected_hit_point.normal, actual_hit_point.normal, 0.001);
                }
            }
        }
    }
}