#ifndef __RTC__CORE__SCATTER_RECORD_H__
#define __RTC__CORE__SCATTER_RECORD_H__

#include "color.h"
#include <math/ray.h>
#include <math/types.h>

namespace ray_tracing_core
{
	namespace material
	{
		class Material;
	}

	namespace core
	{
		class ScatterRecord
		{
		public:

			math::Ray ray;
			bool is_specular;
			Color attenuation;
			math::AlphaValue alpha;
			// ProbabilityDensityFunction
			const material::Material *material;

			static inline ScatterRecord empty(void);
		};

		ScatterRecord ScatterRecord::empty(void)
		{
			return ScatterRecord
			{
				ray : math::Ray::new_ray(math::Point3D(0), math::Vector3D(0)),
				is_specular : false,
				alpha : 0,
				material : nullptr
			};
		}
	}
}

#endif
