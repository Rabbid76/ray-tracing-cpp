#ifndef __RTC__CORE__SCATTER_RECORD_H__
#define __RTC__CORE__SCATTER_RECORD_H__

#include "color.h"
#include <pdf/probability_density_function.h>
#include <math/ray.h>
#include <math/types.h>
#include <memory>

namespace ray_tracing_core {
    namespace material {
        class Material;
    }

    namespace core {
        class ScatterRecord {
        public:
            math::Ray ray;
            bool is_specular{false};
            Color attenuation{Color(0)};
            math::AlphaValue alpha{0.0f};
            std::unique_ptr<pdf::ProbabilityDensityFunction> probability_density_function;
            const material::Material *material = nullptr;

            static inline ScatterRecord empty(void);
        };

        ScatterRecord ScatterRecord::empty(void) {
            return ScatterRecord
                    {
                            .ray = math::Ray::new_ray(math::Point3D(0), math::Vector3D(0)),
                            .is_specular = false,
                            .alpha = 0,
                            .probability_density_function = nullptr,
                            .material = nullptr
                    };
        }
    }
}

#endif
