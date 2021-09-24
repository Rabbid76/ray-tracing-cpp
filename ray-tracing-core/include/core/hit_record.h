#ifndef __RTC__CORE__HIT_RECORD_H__
#define __RTC__CORE__HIT_RECORD_H__

#include <math/types.h>

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

			math::Distance distance;
			math::Point3D position;
			math::Vector3D normal;
			const material::Material *material;
			const ScatterRecord *scatter_record;

			static inline HitRecord empty(void);
		};

		HitRecord HitRecord::empty(void)
		{
			return HitRecord
			{
				distance : 0,
				position : math::Point3D(0),
				normal : math::Vector3D(0),
				material : nullptr,           // TODO: static NoMaterial
				scatter_record : nullptr
			};
		}
	}
}

#endif
