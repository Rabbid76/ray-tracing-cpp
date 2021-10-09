#ifndef __RTC__MATERIAL__LAMBERTIAN_MATERIAL_H__
#define __RTC__MATERIAL__LAMBERTIAN_MATERIAL_H__

#include "material.h"
#include "core/scatter_record.h"
#include <texture/texture.h>
#include <math/random.h>
#include <math/constants.h>
#include <math/ortho_normal_base.h>

namespace ray_tracing_core
{
	namespace material
	{
		class LambertianMaterial
			: public Material
		{
		private:

			const texture::Texture* albedo_texture;

		public:

			inline LambertianMaterial(const texture::Texture* albedo);
			virtual ~LambertianMaterial() = default;
			inline virtual bool hit(core::HitRecord &hit_record) const override;
			inline virtual bool scatter(const math::Ray &ray, core::HitRecord &hit_record, core::ScatterRecord &scatter_record) const override;
			inline virtual math::Distance scattering_pdf(const math::Ray &ray_in, const core::HitRecord &hit_recocrd, const math::Ray &scattered) const override;
			inline virtual core::Color emitt(const math::Ray &ray_in, const core::HitRecord &hit_record) const override;
			inline virtual bool has_texture(void) const override;
			inline virtual bool has_mask(void) const override;
		};

		inline LambertianMaterial::LambertianMaterial(const texture::Texture* albedo)
			: albedo_texture(albedo)
		{}

		bool LambertianMaterial::hit(core::HitRecord &hit_record) const
		{
			if (!has_mask())
				return true;
			auto [color, alpha_value] = albedo_texture->channels(hit_record.texture_coordinate, hit_record.hit_point.position);
			return alpha_value > math::RandomGenerator().random_size();
		}

		bool LambertianMaterial::scatter(const math::Ray &ray, core::HitRecord &hit_record, core::ScatterRecord &scatter_record) const
		{
			auto uvw = math::OrthoNormalBase::from_normal(hit_record.hit_point.normal);
			auto direction = math::normalize(uvw.transform(math::RandomGenerator().random_cosine_direction()));
			auto [albedo_color, alpha_value] = albedo_texture->channels(hit_record.texture_coordinate, hit_record.hit_point.position);
			scatter_record.ray = math::Ray::new_ray(hit_record.hit_point.position, direction); // new_ray_with_attributes
			scatter_record.attenuation = albedo_color; // albedo.truncate(3)
			scatter_record.alpha = alpha_value;
			// scatter_record.probability_density_function : CosinePdf::from_w(&hit_record.normal)
			scatter_record.material = this;
			return true;
		}

		math::Distance LambertianMaterial::scattering_pdf(const math::Ray &ray_in, const core::HitRecord &hit_recocrd, const math::Ray &scattered) const
		{
			auto n_dot_d = math::dot(hit_recocrd.hit_point.normal, normalize(scattered.direction));
			return n_dot_d < 0.0 ? 0.0 : n_dot_d / math::pi<math::Distance>;
		}

		core::Color LambertianMaterial::emitt(const math::Ray &ray_in, const core::HitRecord &hit_record) const
		{
			return core::Color(0);
		}

		bool LambertianMaterial::has_texture(void) const
		{
			return albedo_texture->depends_on_texture_coordinates();
		}

		bool LambertianMaterial::has_mask(void) const
		{
			return albedo_texture->has_alpha_channel();
		}
	}
}

#endif
