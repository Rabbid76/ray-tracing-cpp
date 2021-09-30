#ifndef __RTC__CORE__HIT_RECORD_H__
#define __RTC__CORE__HIT_RECORD_H__

#include <math/types.h>
#include <math/hit_point.h>
#include "texture_coordinate.h"

namespace ray_tracing_core
{
	namespace material
	{
		class Material;
	}

	namespace core
	{
		class HitRecord
		{
		public:

			math::HitPoint hit_point;
			core::TextureCoordinate texture_coordinate;
			const material::Material *material;
			bool scatter_record_is_set;

			static inline HitRecord empty(void);
		};

		HitRecord HitRecord::empty(void)
		{
			return HitRecord
			{
				hit_point : math::HitPoint::new_hit(0, math::Point3D(0), math::Vector3D(0)),
				texture_coordinate : core::TextureCoordinate::null(),
				material : nullptr,
				scatter_record_is_set : false,
			};
		}
	}
}

#endif
