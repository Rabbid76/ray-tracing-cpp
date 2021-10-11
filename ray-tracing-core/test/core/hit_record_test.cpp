#include "hit_record_test.h"
#include <core/hit_record.h>
#include <rtc_core_test_assert.h>

namespace ray_tracing_core_unit_test
{
    namespace core
    {
        using namespace ray_tracing_core::core;
        using namespace ray_tracing_core;

        void hit_record_empty_test(void)
        {
            auto hit_record = HitRecord::empty();
            TEST_ASSERT_EQUAL(0, hit_record.hit_point.distance);
            assert_equal_point(math::Point3D(0), hit_record.hit_point.position, 0);
            assert_equal_vector(math::Vector3D(0), hit_record.hit_point.normal, 0);
            assert_equal_texture_coordinate(TextureCoordinate{ 0 ,0 }, hit_record.texture_coordinate, 0);
            TEST_ASSERT_EQUAL(nullptr, hit_record.material);
        }
    }
}
