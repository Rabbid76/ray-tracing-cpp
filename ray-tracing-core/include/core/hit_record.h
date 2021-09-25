#ifndef __RTC__CORE__HIT_RECORD_H__
#define __RTC__CORE__HIT_RECORD_H__

#include <math/types.h>
#include <math/hit_point.h>

namespace ray_tracing_core
{
	namespace material
	{
		class Material;
	}

	namespace core
	{
		class ScatterRecord;

		class HitRecord
		{
		public:

			math::HitPoint hit_point;
			const material::Material *material;
			const ScatterRecord *scatter_record;

			static inline HitRecord empty(void);
		};

		HitRecord HitRecord::empty(void)
		{
			return HitRecord
			{
				hit_point : math::HitPoint::new_hit(0, math::Point3D(0), math::Vector3D(0)),
				material : nullptr,
				scatter_record : nullptr
			};
		}
	}
}

#endif
