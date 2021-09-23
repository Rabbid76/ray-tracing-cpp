#ifndef __RTC__MATH__AXIS_ALIGNED_BOUNDIG_BOX_H__
#define __RTC__MATH__AXID_ALIGNED_BOUNDIG_BOX_H__

#include "types.h"

namespace ray_tracing_core
{
	namespace math
	{
		class AxisAlignedBoundingBox;
		using AABB = AxisAlignedBoundingBox;

		class AxisAlignedBoundingBox
		{
		public:

			Point3D minimum_point;
			Point3D maximum_point;

			static inline AxisAlignedBoundingBox new_box(const Point3D &pt1, const Point3D &pt2);
		};

		AxisAlignedBoundingBox AxisAlignedBoundingBox::new_box(const Point3D &pt1, const Point3D &pt2)
		{
			return AxisAlignedBoundingBox
			{
				minimum_point : minimum(pt1, pt2),
				maximum_point : maximum(pt1, pt2)
			};
		}
	}
}

#endif
