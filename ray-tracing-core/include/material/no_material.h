#ifndef __RTC__MATERIAL__NO_MATERIAL_H__
#define __RTC__MATERIAL__NO_MATERIAL_H__

#include "material.h"

namespace ray_tracing_core
{
	namespace material
	{
		class NoMaterial
			: public Material
		{
		public:

			virtual ~NoMaterial() = default;
			virtual bool scatter(const math::Ray &ray, core::HitRecord &hit_record, core::ScatterRecord &scatter_record) const override;
			virtual math::Distance scattering_pdf(const math::Ray &ray_in, const core::HitRecord &hit_recocrd, const math::Ray &scattered) const override;
			virtual core::Color emitt(const math::Ray &ray_in, const core::HitRecord &hit_record) const override;
			virtual bool has_alpha(void) const override;
		};

		bool NoMaterial::scatter(const math::Ray &ray, core::HitRecord &hit_record, core::ScatterRecord &scatter_record) const
		{
			return false;
		}

		math::Distance NoMaterial::scattering_pdf(const math::Ray &ray_in, const core::HitRecord &hit_recocrd, const math::Ray &scattered) const
		{
			return 1.0;
		}

		core::Color NoMaterial::emitt(const math::Ray &ray_in, const core::HitRecord &hit_record) const
		{
			return core::Color(0);
		}

		bool NoMaterial::has_alpha(void) const
		{
			return true;
		}
	}
}

#endif
