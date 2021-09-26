#ifndef __RTC_UT__TEST_ASSERT_HELPER_H__
#define __RTC_UT__TEST_ASSERT_HELPER_H__

#include <math/types.h>
#include <core/color.h>
#include <cute.h>

inline void assert_equal_point(
		const ray_tracing_core::math::Vector3D &expected_point,
		const ray_tracing_core::math::Vector3D &actual_point,
		ray_tracing_core::math::Distance delta)
{
	ASSERT_EQUAL_DELTA(expected_point.x, actual_point.x, delta);
	ASSERT_EQUAL_DELTA(expected_point.y, actual_point.y, delta);
	ASSERT_EQUAL_DELTA(expected_point.z, actual_point.z, delta);
}

inline void assert_equal_vector(
		const ray_tracing_core::math::Vector3D &expected_vector,
		const ray_tracing_core::math::Vector3D &actual_vector,
		ray_tracing_core::math::Distance delta)
{
	ASSERT_EQUAL_DELTA(expected_vector.x, actual_vector.x, delta);
	ASSERT_EQUAL_DELTA(expected_vector.y, actual_vector.y, delta);
	ASSERT_EQUAL_DELTA(expected_vector.z, actual_vector.z, delta);
}

inline void assert_equal_color(
		const ray_tracing_core::core::Color &expected_color,
		const ray_tracing_core::core::Color &actual_color,
		float delta)
{
	ASSERT_EQUAL_DELTA(expected_color.r, actual_color.r, delta);
	ASSERT_EQUAL_DELTA(expected_color.g, actual_color.g, delta);
	ASSERT_EQUAL_DELTA(expected_color.b, actual_color.b, delta);
}

#endif
