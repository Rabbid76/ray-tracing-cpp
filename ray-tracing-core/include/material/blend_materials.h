#ifndef __RTC__MATERIAL__BLEND_MATERIALS_H__
#define __RTC__MATERIAL__BLEND_MATERIALS_H__

#include "material.h"
#include <math/random.h>
#include <math/constants.h>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <tuple>
#include <vector>

namespace ray_tracing_core::material
{
    class BlendMaterials
            : public Material
    {
    public:
        using Materials = std::vector<std::pair<double, const Material*>>;

    private:
        Materials materials;

    public:

        inline explicit BlendMaterials(const Materials &materials_and_weights);
        ~BlendMaterials() override = default;
        inline const Material *material() const override;
        inline bool hit(const core::HitRecord &hit_record) const override;
        inline bool scatter(const math::Ray &ray, core::HitRecord &hit_record, core::ScatterRecord &scatter_record) const override;
        inline math::Distance scattering_pdf(const math::Ray &ray_in, const core::HitRecord &hit_recocrd, const math::Ray &scattered) const override;
        inline core::Color emit(const math::Ray &ray_in, const core::HitRecord &hit_record) const override;
        inline bool has_texture() const override;
        inline bool has_mask() const override;
    };

    BlendMaterials::BlendMaterials(const Materials &materials_and_weights)
    {
        double sum = std::accumulate(
                materials_and_weights.begin(), materials_and_weights.end(), 0.0,
                [](const double &a, const Materials::value_type &b) -> double
                {
                    return a + b.first;
                });
        materials = materials_and_weights;
        double current_sum = 0.0;
        for (auto &material : materials)
        {
            current_sum += material.first;
            material.first = current_sum / sum;
        }
    }

    const Material *BlendMaterials::material() const
    {
        auto random_weight = math::RandomGenerator().random_size();
        for (auto &material : materials)
        {
            if (random_weight < material.first)
                return material.second;
        }
        return materials.back().second;
    }

    bool BlendMaterials::hit(const core::HitRecord &hit_record) const
    {
        throw std::runtime_error("internal error");
    }

    bool BlendMaterials::scatter(const math::Ray &ray, core::HitRecord &hit_record, core::ScatterRecord &scatter_record) const
    {
        throw std::runtime_error("internal error");
    }

    math::Distance BlendMaterials::scattering_pdf(const math::Ray &ray_in, const core::HitRecord &hit_recocrd, const math::Ray &scattered) const
    {
        throw std::runtime_error("internal error");
    }

    core::Color BlendMaterials::emit(const math::Ray &ray_in, const core::HitRecord &hit_record) const
    {
        throw std::runtime_error("internal error");
    }

    bool BlendMaterials::has_texture() const
    {
        throw std::runtime_error("internal error");
    }

    bool BlendMaterials::has_mask() const
    {
        throw std::runtime_error("internal error");
    }
}

#endif