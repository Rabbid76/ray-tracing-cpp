#ifndef __RTC__MATH__RECTANGLE_H__
#define __RTC__MATH__RECTANGLE_H__

#include "math/axis_aligned_bounding_box.h"
#include "math/hit_point.h"
#include "math/ray.h"
#include "math/types.h"
#include <algorithm>
#include <cmath>
#include <cstdint>

namespace ray_tracing_core::math
{
	class Rectangle
	{
	public:
		enum class Orientation : uint32_t
		{
			YZ=0, XZ=1, XY=2
		};

		Orientation orientation;
		Distance k;
		DistanceRange rangeA;
		DistanceRange rangeB;

		inline static Rectangle new_rectangle(Orientation orientation, Distance k, const Point2D& a, const Point2D& b);
		inline AxisAlignedBoundingBox bounding_box(void) const;
		inline bool hit(const Ray& ray, const DistanceRange& distance_range, HitPoint& hit_point) const;
		inline Vector3D normal_vector(const Point3D& point_on_sphere) const;
	};

	Rectangle Rectangle::new_rectangle(Orientation orientation, Distance k, const Point2D& a, const Point2D& b)
	{
		return Rectangle
		{
			.orientation = orientation,
			.k = k,
			.rangeA = { std::min(a.x, b.x), std::max(a.x, b.x) },
			.rangeB = { std::min(a.y, b.y), std::max(a.y, b.y) },
		};
	}

	AxisAlignedBoundingBox Rectangle::bounding_box(void) const
	{
		auto& [a0, a1] = rangeA;
		auto& [b0, b1] = rangeB;
		switch (orientation)
		{
		case Orientation::XY: return AxisAlignedBoundingBox::new_box(Point3D{ a0, b0, k - 0.0001 }, Point3D{ a1, b1, k + 0.0001 });
		case Orientation::YZ: return AxisAlignedBoundingBox::new_box(Point3D{ k - 0.0001, a0, b0 }, Point3D{ k + 0.0001, a1, b1 });
		case Orientation::XZ: return AxisAlignedBoundingBox::new_box(Point3D{ a0, k - 0.0001, b0 }, Point3D{ a1, k + 0.0001, b1 });
		}
	}

	bool Rectangle::hit(const Ray& ray, const DistanceRange& distance_range, HitPoint& hit_point) const
	{
		auto [from, to] = distance_range;
		if (ray.direction[(uint32_t)orientation] == 0)
			return false;
		auto t = (k - ray.origin[(uint32_t)orientation]) / ray.direction[(uint32_t)orientation];
		if (t < from || t > to)
			return false;

		auto& [a0, a1] = rangeA;
		auto& [b0, b1] = rangeB;
		uint32_t iA = (orientation == Orientation::YZ) ? 1 : 0;
		uint32_t iB = (orientation == Orientation::XY) ? 1 : 2;
		auto a = ray.origin[iA] + t * ray.direction[iA];
		auto b = ray.origin[iB] + t * ray.direction[iB];
		if (a < a0 || a > a1 || b < b0 || b > b1)
			return false;

		hit_point.distance = t;
		hit_point.position = ray.point_at(t);
		hit_point.normal = normal_vector(hit_point.position);
		return true;
	}

	Vector3D Rectangle::normal_vector(const Point3D& point_on_sphere) const
	{
		switch (orientation)
		{
		case Orientation::XY: return Vector3D(0, 0, -1);
		case Orientation::YZ: return Vector3D(-1, 0, 0);
		case Orientation::XZ: return Vector3D(0, -1, 0);
		}
	}
}

#endif