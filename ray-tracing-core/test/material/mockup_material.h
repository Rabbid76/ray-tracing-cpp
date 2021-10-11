#ifndef __RTC_UT__MATERIAL__MOCKUP_MATERIAL_H__
#define __RTC_UT__MATERIAL__MOCKUP_MATERIAL_H__

#include <material/material.h>

namespace ray_tracing_core_unit_test
{
    namespace material
    {
        using namespace ray_tracing_core::material;
        using namespace ray_tracing_core;

        class MockupMaterial
            : public Material
        {
        public:

            bool hit_result = false;
            bool scatter_result = false;
            math::Distance scattering_pdf_result = 0;
            ray_tracing_core::core::Color emitt_result = ray_tracing_core::core::Color(0);
            bool has_texture_result = false;
            bool has_mask_result = false;

            virtual ~MockupMaterial() = default;

            virtual bool hit(ray_tracing_core::core::HitRecord& hit_record) const override
            {
                return hit_result;
            }

            virtual bool scatter(const math::Ray& ray, ray_tracing_core::core::HitRecord& hit_record, ray_tracing_core::core::ScatterRecord& scatter_record) const override
            {
                return scatter_result;
            }

            virtual math::Distance scattering_pdf(const math::Ray& ray_in, const ray_tracing_core::core::HitRecord& hit_recocrd, const math::Ray& scattered) const override
            {
                return scattering_pdf_result;
            }

            virtual ray_tracing_core::core::Color emitt(const math::Ray& ray_in, const ray_tracing_core::core::HitRecord& hit_record) const override
            {
                return emitt_result;
            }

            virtual bool has_texture(void) const override
            {
                return has_texture_result;
            }

            virtual bool has_mask(void) const override
            {
                return has_mask_result;
            }
        };
    }
}

#endif
