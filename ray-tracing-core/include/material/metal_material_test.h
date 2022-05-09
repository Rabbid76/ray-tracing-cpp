#ifndef __RTC__MATERIAL__METAL_MATERIAL_H__
#define __RTC__MATERIAL__METAL_MATERIAL_H__

#include "material.h"
#include "core/scatter_record.h"
#include <texture/texture.h>
#include <math/random.h>
#include <math/constants.h>
#include <math/ortho_normal_base.h>
#include <pdf/cosine_pdf.h>

namespace ray_tracing_core::material
{
    class MetalMaterial
            : public Material
    {
    private:

        double fuzz;
        const texture::Texture* albedo_texture;

    public:

        inline explicit MetalMaterial(double fuzz, const texture::Texture* albedo);
        ~MetalMaterial() override = default;
        inline bool hit(const core::HitRecord &hit_record) const override;
        inline bool scatter(const math::Ray &ray, core::HitRecord &hit_record, core::ScatterRecord &scatter_record) const override;
        inline math::Distance scattering_pdf(const math::Ray &ray_in, const core::HitRecord &hit_recocrd, const math::Ray &scattered) const override;
        inline core::Color emitt(const math::Ray &ray_in, const core::HitRecord &hit_record) const override;
        inline bool has_texture() const override;
        inline bool has_mask() const override;
    };

    inline MetalMaterial::MetalMaterial(double fuzz, const texture::Texture* albedo)
            : albedo_texture(albedo)
    {}

    bool MetalMaterial::hit(const core::HitRecord &hit_record) const
    {
        if (!has_mask())
            return true;
        auto [color, alpha_value] = albedo_texture->channels(hit_record.texture_coordinate, hit_record.hit_point.position);
        return alpha_value > math::RandomGenerator().random_size();
    }

    bool MetalMaterial::scatter(const math::Ray &ray, core::HitRecord &hit_record, core::ScatterRecord &scatter_record) const
    {
        auto nv = hit_record.hit_point.normal * -glm::sign(glm::dot(ray.direction, hit_record.hit_point.normal));
        auto reflected = glm::reflect(ray.direction, nv);
        auto scattered_ray = math::Ray::new_ray_with_attributes(
                hit_record.hit_point.position, reflected + math::RandomGenerator().random_in_unit_sphere() * fuzz, ray);
        if (glm::dot(scattered_ray.direction, nv) > 0.0)
        {
            auto [albedo_color, alpha_value] = albedo_texture->channels(hit_record.texture_coordinate, hit_record.hit_point.position);
            scatter_record.ray = scattered_ray;
            scatter_record.is_specular = true;
            scatter_record.attenuation = albedo_color;
            scatter_record.alpha = alpha_value;
            scatter_record.probability_density_function =
                    std::make_unique<pdf::CosinePDF>(pdf::CosinePDF::from_normal(hit_record.hit_point.normal));
            scatter_record.material = this;
            return true;
        }
        return false;
    }

    math::Distance MetalMaterial::scattering_pdf(const math::Ray &ray_in, const core::HitRecord &hit_recocrd, const math::Ray &scattered) const
    {
        return 1.0;
    }

    core::Color MetalMaterial::emitt(const math::Ray &ray_in, const core::HitRecord &hit_record) const
    {
        return core::Color(0);
    }

    bool MetalMaterial::has_texture() const
    {
        return albedo_texture->depends_on_texture_coordinates();
    }

    bool MetalMaterial::has_mask() const
    {
        return albedo_texture->has_alpha_channel();
    }
}

#endif