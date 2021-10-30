#ifndef __RTC_TEST_ASSERT_H__
#define __RTC_TEST_ASSERT_H__

#include "test_assert.h"
#include <math/types.h>
#include <core/color.h>
#include <core/texture_coordinate.h>

inline void assert_equal_point(
	const ray_tracing_core::math::Vector3D& expected_point,
	const ray_tracing_core::math::Vector3D& actual_point,
	ray_tracing_core::math::Distance delta)
{
	TEST_ASSERT_EQUAL_DELTA(expected_point.x, actual_point.x, delta);
	TEST_ASSERT_EQUAL_DELTA(expected_point.y, actual_point.y, delta);
	TEST_ASSERT_EQUAL_DELTA(expected_point.z, actual_point.z, delta);
}

inline void assert_equal_vector(
	const ray_tracing_core::math::Vector3D& expected_vector,
	const ray_tracing_core::math::Vector3D& actual_vector,
	ray_tracing_core::math::Distance delta)
{
	TEST_ASSERT_EQUAL_DELTA(expected_vector.x, actual_vector.x, delta);
	TEST_ASSERT_EQUAL_DELTA(expected_vector.y, actual_vector.y, delta);
	TEST_ASSERT_EQUAL_DELTA(expected_vector.z, actual_vector.z, delta);
}

inline void assert_equal_texture_coordinate(
	const ray_tracing_core::core::TextureCoordinate& expected_coordinate,
	const ray_tracing_core::core::TextureCoordinate& actual_coordinate,
	ray_tracing_core::math::TextureDistance delta)
{
	TEST_ASSERT_EQUAL_DELTA(expected_coordinate.u, actual_coordinate.u, delta);
	TEST_ASSERT_EQUAL_DELTA(expected_coordinate.v, actual_coordinate.v, delta);
}

inline void assert_equal_color(
	const ray_tracing_core::core::Color& expected_color,
	const ray_tracing_core::core::Color& actual_color,
	float delta)
{
	TEST_ASSERT_EQUAL_DELTA(expected_color.r, actual_color.r, delta);
	TEST_ASSERT_EQUAL_DELTA(expected_color.g, actual_color.g, delta);
	TEST_ASSERT_EQUAL_DELTA(expected_color.b, actual_color.b, delta);
}

inline void assert_equal_distance_range(
	const ray_tracing_core::math::DistanceRange& expected_range,
	const ray_tracing_core::math::DistanceRange& actual_range,
	ray_tracing_core::math::TextureDistance delta)
{
	TEST_ASSERT_EQUAL_DELTA(std::get<0>(expected_range), std::get<0>(actual_range), delta);
	TEST_ASSERT_EQUAL_DELTA(std::get<1>(expected_range), std::get<1>(actual_range), delta);
}

inline void assert_equal_time_range(
	const ray_tracing_core::math::TimeRange& expected_range,
	const ray_tracing_core::math::TimeRange& actual_range,
	ray_tracing_core::math::Time delta)
{
	TEST_ASSERT_EQUAL_DELTA(std::get<0>(expected_range), std::get<0>(actual_range), delta);
	TEST_ASSERT_EQUAL_DELTA(std::get<1>(expected_range), std::get<1>(actual_range), delta);
}

#endif