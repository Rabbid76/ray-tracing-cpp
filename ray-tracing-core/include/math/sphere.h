#ifndef __RTC__MATH__SPHERE_H__
#define __RTC__MATH__SPHERE_H__

#include "types.h"
#include "axis_aligned_bounding_box.h"

namespace ray_tracing_core
{
	namespace math
	{
		class Sphere
		{
		public:

			Point3D center;
			Distance radius;

			static inline Sphere new_sphere(const Point3D &sphere_cente, const Distance &radius);
			inline AxisAlignedBoundingBox bounding_box(void) const;
		};

		Sphere Sphere::new_sphere(const Point3D &sphere_center, const Distance &sphere_radius)
		{
			return Sphere
			{
				center : sphere_center,
			    radius : sphere_radius,
			};
		}

		AxisAlignedBoundingBox Sphere::bounding_box(void) const
		{
			return AxisAlignedBoundingBox::new_box(
				center - Vector3D(radius),
				center + Vector3D(radius)
			);
		}
	}
}

#endif
