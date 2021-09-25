#include "hit_record_test.h"
#include "cute.h"
#include <core/hit_record.h>

namespace core
{
	using namespace ray_tracing_core::core;

	void hit_record_empty_test(void)
	{
		auto hit_record = HitRecord::empty();
		ASSERT_EQUAL(0, hit_record.hit_point.distance);
		ASSERT_EQUAL(0, hit_record.hit_point.position.x);
		ASSERT_EQUAL(0, hit_record.hit_point.position.y);
		ASSERT_EQUAL(0, hit_record.hit_point.position.z);
		ASSERT_EQUAL(0, hit_record.hit_point.normal.x);
		ASSERT_EQUAL(0, hit_record.hit_point.normal.y);
		ASSERT_EQUAL(0, hit_record.hit_point.normal.z);
		ASSERT_EQUAL(nullptr, hit_record.material);
		ASSERT_EQUAL(nullptr, hit_record.scatter_record);
	}
}
