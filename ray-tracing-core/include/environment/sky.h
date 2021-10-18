#ifndef __RTC__ENVIRONMENT__SKY_H__
#define __RTC__ENVIRONMENT__SKY_H__

#include <core/color.h>
#include <math/ray.h>

namespace ray_tracing_core
{
    namespace environemnt
    {
        class Sky
        {
        public:

            core::Color nadir_color;
            core::Color zenith_color;

            static inline Sky new_sky(const core::Color& nadir, const core::Color& zenith);
            inline core::Color color_at(const math::Ray& ray);
        };

        Sky Sky::new_sky(const core::Color& nadir, const core::Color& zenith)
        {
            return Sky
            {
                .nadir_color = nadir,
                .zenith_color = zenith
            };
        }

        core::Color Sky::color_at(const math::Ray& ray)
        {
            auto unit_direction = math::normalize(ray.direction);
            auto t = (math::AlphaValue)(unit_direction.y * 0.5 + 0.5);
            return nadir_color * (1.0f - t) + zenith_color * t;
        }
    }
}

#endif                                                                                                                                       enci