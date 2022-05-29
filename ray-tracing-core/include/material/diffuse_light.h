#ifndef __RTC__MATERIAL__DIFFUSE_LIGHT_H__
#define __RTC__MATERIAL__DIFFUSE_LIGHT_H__

#include "material.h"
#include "core/scatter_record.h"
#include <texture/texture.h>
#include <math/random.h>
#include <math/constants.h>
#include <math/ortho_normal_base.h>
#include <pdf/cosine_pdf.h>

namespace ray_tracing_core::material
{
    class DiffuseLight
            : public Material
    {
    private:

        const texture::Texture* emit_texture;

    public:

        inline explicit DiffuseLight(const texture::Texture* emit);
        ~DiffuseLight() override = default;
        inline bool hit(const core::HitRecord &hit_record) const override;
        inline bool scatter(const math::Ray &ray, core::HitRecord &hit_record, core::ScatterRecord &scatter_record) const override;
        inline math::Distance scattering_pdf(const math::Ray &ray_in, const core::HitRecord &hit_recocrd, const math::Ray &scattered) const override;
        inline core::Color emit(const math::Ray &ray_in, const core::HitRecord &hit_record) const override;
        inline bool has_texture() const override;
        inline bool has_mask() const override;
    };

    DiffuseLight::DiffuseLight(const texture::Texture* emit)
            : emit_texture(emit)
    {}

    bool DiffuseLight::hit(const core::HitRecord &hit_record) const
    {
        if (!has_mask())
            return true;
        auto [color, alpha_value] = emit_texture->channels(hit_record.texture_coordinate, hit_record.hit_point.position);
        return alpha_value > math::RandomGenerator().random_size();
    }

    bool DiffuseLight::scatter(const math::Ray &ray, core::HitRecord &hit_record, core::ScatterRecord &scatter_record) const
    {
        return false;
    }

    math::Distance DiffuseLight::scattering_pdf(const math::Ray &ray_in, const core::HitRecord &hit_recocrd, const math::Ray &scattered) const
    {
        return 1.0;
    }

    core::Color DiffuseLight::emit(const math::Ray &ray_in, const core::HitRecord &hit_record) const
    {
        if (math::dot(ray_in.direction, hit_record.hit_point.normal) > 0)
            return core::Color(0);
        auto [color, alpha_value] = emit_texture->channels(hit_record.texture_coordinate, hit_record.hit_point.position);
        return color;

    }

    bool DiffuseLight::has_texture() const
    {
        return emit_texture->depends_on_texture_coordinates();
    }

    bool DiffuseLight::has_mask() const
    {
        return emit_texture->has_alpha_channel();
    }
}

#endif