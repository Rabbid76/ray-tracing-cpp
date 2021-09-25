#ifndef __RTC__MATH__HIT_POINT_H__
#define __RTC__MATH__HIT_POINT_H__

#include "types.h"

namespace ray_tracing_core
{
	namespace math
	{
		class HitPoint
		{
		public:

			Distance distance;
			Point3D position;
			Vector3D normal;

			static inline HitPoint new_hit(Distance distance, const Point3D &hit_point, const Vector3D &normal_vector);
		};

		HitPoint HitPoint::new_hit(Distance distance, const Point3D &hit_point, const Vector3D &normal_vector)
		{
			return HitPoint
			{
				distance : distance,
				position : hit_point,
				normal : normal_vector
			};
		}
	}
}

#endif
