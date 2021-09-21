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
		};

		Ray Ray::new_ray(const Point3D &ray_origin, const Vector3D &ray_direction)
		{
			return Ray
			{
				origin : ray_origin,
				direction : ray_direction,
				time : 0,
			};
		}
	}
}

#endif
