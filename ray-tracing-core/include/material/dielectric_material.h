#ifndef __RTC__MATERIAL__DIELECTRIC_MATERIAL_H__
#define __RTC__MATERIAL__DIELECTRIC_MATERIAL_H__

#include "material.h"
#include "core/scatter_record.h"
#include <texture/texture.h>
#include <math/functions.h>
#include <math/random.h>
#include <math/constants.h>
#include <math/ortho_normal_base.h>
#include <pdf/cosine_pdf.h>

namespace ray_tracing_core::material
{
    class DielectricMaterial
            : public Material
    {
    private:

        math::DistanceRange refraction_index;
        const texture::Texture* albedo_texture;

    public:

        inline explicit DielectricMaterial(const math::DistanceRange &refraction_index, const texture::Texture* albedo);
        ~DielectricMaterial() override = default;
        inline bool hit(const core::HitRecord &hit_record) const override;
        inline bool scatter(const math::Ray &ray, core::HitRecord &hit_record, core::ScatterRecord &scatter_record) const override;
        inline math::Distance scattering_pdf(const math::Ray &ray_in, const core::HitRecord &hit_recocrd, const math::Ray &scattered) const override;
        inline core::Color emit(const math::Ray &ray_in, const core::HitRecord &hit_record) const override;
        inline bool has_texture() const override;
        inline bool has_mask() const override;

    private:
        core::Color hue_to_rgb(float h);
    };

    DielectricMaterial::DielectricMaterial(const math::DistanceRange &refraction_index, const texture::Texture* albedo)
            : refraction_index(refraction_index)
            , albedo_texture(albedo)
    {}

    bool DielectricMaterial::hit(const core::HitRecord &hit_record) const
    {
        if (!has_mask())
            return true;
        auto [color, alpha_value] = albedo_texture->channels(hit_record.texture_coordinate, hit_record.hit_point.position);
        return alpha_value > math::RandomGenerator().random_size();
    }

    bool DielectricMaterial::scatter(const math::Ray &ray, core::HitRecord &hit_record, core::ScatterRecord &scatter_record) const
    {
        auto r_dot_n = glm::dot(ray.direction, hit_record.hit_point.normal);
        // TODO
        /*
        let mut w = ray_in.w;
        if self.ref_idx.end > self.ref_idx.start + 0.00001 && w.is_none() {
            let w_value = random::generate_size();
            albedo = albedo * Dielectric::hue_to_rgb(glm::clamp((w_value) * 5.0 / 6.0, 0.0, 1.0));
            w = Some(w_value);
        }
        let ref_idx = match w {
            Some(w_value) => glm::mix(self.ref_idx.start, self.ref_idx.end, w_value),
            None => self.ref_idx.start,
        };
        */
        auto ref_idx = std::get<0>(refraction_index);
        auto outward_normal = r_dot_n > 0.0 ? -hit_record.hit_point.normal : hit_record.hit_point.normal;
        auto ni_over_nt = r_dot_n > 0.0 ? ref_idx : 1.0f / ref_idx;
        auto cosine = r_dot_n > 0.0 ? ref_idx * r_dot_n / glm::length(ray.direction) : -r_dot_n / glm::length(ray.direction);
        math::Vector3D direction;
        if (auto refracted = math::refract(ray.direction, outward_normal, ni_over_nt))
        {
            auto reflect_probe = math::schlick(cosine, ref_idx);
            direction = math::RandomGenerator().random_size() < reflect_probe
                    ? glm::reflect(ray.direction, hit_record.hit_point.normal)
                    : refracted.value();
        }
        else
        {
            direction = glm::reflect(ray.direction, hit_record.hit_point.normal);
        }

        auto [albedo_color, alpha_value] = albedo_texture->channels(hit_record.texture_coordinate, hit_record.hit_point.position);
        scatter_record.ray = math::Ray::new_ray_with_attributes(hit_record.hit_point.position, direction, ray);
        scatter_record.is_specular = true;
        scatter_record.attenuation = albedo_color;
        scatter_record.alpha = alpha_value;
        scatter_record.probability_density_function = nullptr;
        scatter_record.material = this;
        return true;
    }

    math::Distance DielectricMaterial::scattering_pdf(const math::Ray &ray_in, const core::HitRecord &hit_recocrd, const math::Ray &scattered) const
    {
        return 1.0;
    }

    core::Color DielectricMaterial::emit(const math::Ray &ray_in, const core::HitRecord &hit_record) const
    {
        return core::Color(0);
    }

    bool DielectricMaterial::has_texture() const
    {
        return albedo_texture->depends_on_texture_coordinates();
    }

    bool DielectricMaterial::has_mask() const
    {
        return albedo_texture->has_alpha_channel();
    }

    core::Color hue_to_rgb(float h)
    {
        return core::Color
        {
            glm::clamp(std::fabs(h * 6.0f - 3.0f) - 1.0f, 0.0f, 1.0f),
            glm::clamp(2.0f - std::fabs(h * 6.0f - 2.0f), 0.0f, 1.0f),
            glm::clamp(2.0f - std::fabs(h * 6.0f - 4.0f), 0.0f, 1.0f),
        };
    }
}

#endif