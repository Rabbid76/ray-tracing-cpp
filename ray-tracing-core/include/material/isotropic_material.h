#ifndef __RTC_UT__MATERIAL__ISOTROPIC_MATERIAL_H__
#define __RTC_UT__MATERIAL__ISOTROPIC_MATERIAL_H__

#include "material.h"
#include "core/scatter_record.h"
#include <texture/texture.h>
#include <math/random.h>
#include <math/constants.h>
#include <pdf/cosine_pdf.h>

namespace ray_tracing_core::material
{
    class IsotropicMaterial
            : public Material
    {
    private:

        const texture::Texture* albedo_texture;

    public:

        inline explicit IsotropicMaterial(const texture::Texture* albedo);
        ~IsotropicMaterial() override = default;
        inline bool hit(const core::HitRecord &hit_record) const override;
        inline bool scatter(const math::Ray &ray, core::HitRecord &hit_record, core::ScatterRecord &scatter_record) const override;
        inline math::Distance scattering_pdf(const math::Ray &ray_in, const core::HitRecord &hit_recocrd, const math::Ray &scattered) const override;
        inline core::Color emit(const math::Ray &ray_in, const core::HitRecord &hit_record) const override;
        inline bool has_texture() const override;
        inline bool has_mask() const override;
    };

    IsotropicMaterial::IsotropicMaterial(const texture::Texture* albedo)
            : albedo_texture(albedo)
    {}

    bool IsotropicMaterial::hit(const core::HitRecord &hit_record) const
    {
        if (!has_mask())
            return true;
        auto [color, alpha_value] = albedo_texture->channels(hit_record.texture_coordinate, hit_record.hit_point.position);
        return alpha_value > math::RandomGenerator().random_size();
    }

    bool IsotropicMaterial::scatter(const math::Ray &ray, core::HitRecord &hit_record, core::ScatterRecord &scatter_record) const
    {
        auto [albedo_color, alpha_value] = albedo_texture->channels(hit_record.texture_coordinate, hit_record.hit_point.position);
        scatter_record.ray = math::Ray::new_ray_with_attributes(hit_record.hit_point.position, math::RandomGenerator().random_in_unit_sphere(), ray);
        scatter_record.is_specular = true;
        scatter_record.attenuation = albedo_color;
        scatter_record.alpha = alpha_value;
        scatter_record.probability_density_function = nullptr;
        scatter_record.material = this;
        return true;
    }

    math::Distance IsotropicMaterial::scattering_pdf(const math::Ray &ray_in, const core::HitRecord &hit_recocrd, const math::Ray &scattered) const
    {
        return 1.0;
    }

    core::Color IsotropicMaterial::emit(const math::Ray &ray_in, const core::HitRecord &hit_record) const
    {
        return core::Color(0);
    }

    bool IsotropicMaterial::has_texture() const
    {
        return albedo_texture->depends_on_texture_coordinates();
    }

    bool IsotropicMaterial::has_mask() const
    {
        return albedo_texture->has_alpha_channel();
    }
}

#endif