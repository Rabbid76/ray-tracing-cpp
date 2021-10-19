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
            virtual inline bool hit(const core::HitRecord& hit_record) const override;
            virtual inline bool scatter(const math::Ray& ray, core::HitRecord& hit_record, core::ScatterRecord& scatter_record) const override;
            virtual inline math::Distance scattering_pdf(const math::Ray& ray_in, const core::HitRecord& hit_recocrd, const math::Ray& scattered) const override;
            virtual inline core::Color emitt(const math::Ray& ray_in, const core::HitRecord& hit_record) const override;
            virtual inline bool has_texture(void) const override;
            virtual inline bool has_mask(void) const override;
        };

        bool NoMaterial::hit(const core::HitRecord& hit_record) const
        {
            return true;
        }

        bool NoMaterial::scatter(const math::Ray& ray, core::HitRecord& hit_record, core::ScatterRecord& scatter_record) const
        {
            return false;
        }

        math::Distance NoMaterial::scattering_pdf(const math::Ray& ray_in, const core::HitRecord& hit_recocrd, const math::Ray& scattered) const
        {
            return 1.0;
        }

        core::Color NoMaterial::emitt(const math::Ray& ray_in, const core::HitRecord& hit_record) const
        {
            return core::Color(0);
        }

        bool NoMaterial::has_texture(void) const
        {
            return false;
        }

        bool NoMaterial::has_mask(void) const
        {
            return false;
        }
    }
}

#endif
