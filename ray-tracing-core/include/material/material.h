#ifndef __RTC__MATERIAL__MATERIAL_H__
#define __RTC__MATERIAL__MATERIAL_H__

#include "math/ray.h"
#include "core/hit_record.h"
#include "core/scatter_record.h"
#include "core/color.h"

namespace ray_tracing_core
{
	namespace material
	{
		class Material
		{
		public:
			virtual ~Material() = default;
			virtual bool hit(core::HitRecord &hit_record) const = 0; // hit = color.alpha > random[0, 1]
			virtual bool scatter(const math::Ray &ray, core::HitRecord &hit_record, core::ScatterRecord &scatter_record) const = 0;
			virtual math::Distance scattering_pdf(const math::Ray &ray_in, const core::HitRecord &hit_recocrd, const math::Ray &scattered) const = 0;
			virtual core::Color emitt(const math::Ray &ray_in, const core::HitRecord &hit_record) const = 0;
			virtual bool has_texture(void) const = 0;
			virtual bool has_mask(void) const = 0;
		};
	}
}

#endif
