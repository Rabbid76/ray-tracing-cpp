#ifndef __RTC__MATH__RAY_H__
#define __RTC__MATH__RAY_H__

#include "types.h"
#include <utility>

namespace ray_tracing_core
{
	namespace math
	{
		class Ray
		{
		public:

			Point3D origin;
			Vector3D direction;
			Time time;

			static inline Ray new_ray(const Point3D &ray_origin, const Vector3D &ray_direction);
			static inline Ray new_ray_with_attributes(const Point3D& ray_origin, const Vector3D& ray_direction, const Ray &attributes);

			inline Point3D point_at(Distance distance) const;
		};

		Ray Ray::new_ray(const Point3D &ray_origin, const Vector3D &ray_direction)
		{
			return Ray
			{
				.origin = ray_origin,
				.direction = ray_direction,
				.time = 0
			};
		}

		Ray Ray::new_ray_with_attributes(const Point3D& ray_origin, const Vector3D& ray_direction, const Ray& attributes)
		{
			return Ray
			{
				.origin = ray_origin,
				.direction = ray_direction,
				.time = attributes.time
			};
		}

		Point3D Ray::point_at(Distance distance) const
		{
			return origin + direction * distance;
		}
	}
}

#endif
