#include "hit_record_test.h"
#include "cute.h"
#include <core/hit_record.h>

namespace core
{
	void hit_record_empty_test(void)
	{
		auto hit_record = ray_tracing_core::core::HitRecord::empty();
		ASSERT_EQUAL(0, hit_record.distance);
		ASSERT_EQUAL(0, hit_record.position.x);
		ASSERT_EQUAL(0, hit_record.position.y);
		ASSERT_EQUAL(0, hit_record.position.z);
		ASSERT_EQUAL(0, hit_record.normal.x);
		ASSERT_EQUAL(0, hit_record.normal.y);
		ASSERT_EQUAL(0, hit_record.normal.z);
		ASSERT_EQUAL(nullptr, hit_record.material);
		ASSERT_EQUAL(nullptr, hit_record.scatter_record);
	}
}
