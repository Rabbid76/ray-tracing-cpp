#ifndef __RTC__MATH__ORTHO_NORMAL_BASE_H__
#define __RTC__MATH__ORTHO_NORMAL_BASE_H__

#include "types.h"

namespace ray_tracing_core
{
	namespace math
	{
		class OrthoNormalBase
		{
		public:

			NormalMatrix axis;

			static inline OrthoNormalBase from_normal(const Vector3D &normal);

			const Vector3D &u(void) const { return axis[0]; }
			const Vector3D &v(void) const { return axis[1]; }
			const Vector3D &w(void) const { return axis[2]; }
			Vector3D &u(void) { return axis[0]; }
			Vector3D &v(void) { return axis[1]; }
			Vector3D &w(void) { return axis[2]; }
			inline Vector3D transform(const Vector3D &vecotr) const;
		};

		OrthoNormalBase OrthoNormalBase::from_normal(const Vector3D &normal)
		{
			auto axis_z = normalize(normal);
			auto normal_to_y = std::fabs(axis_z.x) > 0.9 ? Vector3D(0, 1, 0) : Vector3D(1, 0, 0);
			auto axis_y = normalize(cross(axis_z, normal_to_y));
			auto axis_x = cross(axis_z, axis_y);
			return OrthoNormalBase
			{
				.axis = NormalMatrix(axis_x, axis_y, axis_z)
			};
		}

		Vector3D OrthoNormalBase::transform(const Vector3D &vector) const
		{
			return axis * vector;
		}
	}
}

#endif
