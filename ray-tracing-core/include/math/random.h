#ifndef __RTC__MATH__RANDOM_H__
#define __RTC__MATH__RANDOM_H__

#include "types.h"
#include <cstdint>
#include <random>

namespace ray_tracing_core::math
{
    class RandomGenerator
    {
    private:

        static std::default_random_engine generator;
        static std::uniform_real_distribution<Distance> distribution_size;
        static std::uniform_real_distribution<Distance> distribution_unit;
        static std::uniform_int_distribution<uint32_t> distribution_axis;

    public:

        inline std::default_random_engine &get_generator();
        uint32_t random_axis() const;
        Distance random_size() const;
        Distance random_unit() const;
        Vector3D random_unit_vector() const;
        Vector3D random_in_unit_cube() const;
        Vector3D random_in_unit_sphere() const;
        Vector3D random_cosine_direction() const;
        Vector3D random_vector_to_sphere(Distance radius, Distance distance_squared) const;
    };

    inline std::default_random_engine &RandomGenerator::get_generator()
    {
        return generator;
    }
}

#endif
