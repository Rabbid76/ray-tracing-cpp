#include "hit_point_test.h"
#include <math/hit_point.h>
#include <rtc_test_assert.h>

namespace ray_tracing_core_unit_test
{
    namespace math
    {
        using namespace ray_tracing_core::math;

        void hit_point_new_hit_test(void)
        {
            auto hit_point = HitPoint::new_hit(1, Point3D(2, 3, 4), Vector3D(5, 6, 7));
            TEST_ASSERT_EQUAL(1, hit_point.distance);
            assert_equal_point(Point3D(2, 3, 4), hit_point.position, 0);
            assert_equal_vector(Vector3D(5, 6, 7), hit_point.normal, 0);
        }
    }
}
