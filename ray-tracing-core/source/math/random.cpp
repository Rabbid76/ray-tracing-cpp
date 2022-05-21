#include <math/random.h>
#include <math/constants.h>
#include <cmath>
#include <random>


namespace ray_tracing_core
{
    namespace math
    {
        std::default_random_engine RandomGenerator::generator;
        std::uniform_real_distribution<Distance> RandomGenerator::distribution_size =
            std::uniform_real_distribution<Distance>(0.0, 1.0);
        std::uniform_real_distribution<Distance> RandomGenerator::distribution_unit =
            std::uniform_real_distribution<Distance>(-1.0, 1.0);
        std::uniform_int_distribution<uint32_t> RandomGenerator::distribution_axis =
            std::uniform_int_distribution<uint32_t>(0, 2);

        uint32_t RandomGenerator::random_axis() const
        {
            return distribution_axis(generator);
        }

        Distance RandomGenerator::random_size() const
        {
            return distribution_size(generator);
        }

        Distance RandomGenerator::random_unit() const
        {
            return distribution_unit(generator);
        }

        Vector3D RandomGenerator::random_unit_vector() const
        {
            Vector3D random_vector;
            do
            {
                random_vector = Vector3D(random_unit(), random_unit(), random_unit());
            } 
            while (math::dot(random_vector, random_vector) == 0.0);
            return normalize(random_vector);
        }

        Vector3D RandomGenerator::random_in_unit_cube() const
        {
            return Vector3D(random_unit(), random_unit(), random_unit());
        }

        Vector3D RandomGenerator::random_in_unit_sphere() const
        {
            Vector3D random_vector;
            do
            {
                random_vector = Vector3D(random_unit(), random_unit(), random_unit());
            } 
            while (math::dot(random_vector, random_vector) >= 1.0);
            return random_vector;
        }

        Vector3D RandomGenerator::random_cosine_direction() const
        {
            auto r1 = random_size();
            auto r2 = random_size();
            auto z = std::sqrt(1.0 - r2);
            auto phi = 2.0 * pi<Distance> * r1;
            auto x = std::cos(phi) * 2.0 * std::sqrt(r2);
            auto y = std::sin(phi) * 2.0 * std::sqrt(r2);
            return Vector3D(x, y, z);
        }

        Vector3D RandomGenerator::random_vector_to_sphere(Distance radius, Distance distance_squared) const
        {
            auto r1 = random_size();
            auto r2 = random_size();
            auto z = 1.0 + r2 * (std::sqrt(1.0 - radius * radius / distance_squared) - 1.0);
            auto phi = 2.0 * pi<Distance> *r1;
            auto x = std::cos(phi) * std::sqrt(1.0 - z * z);
            auto y = std::sin(phi) * std::sqrt(1.0 - z * z);
            return Vector3D(x, y, z);
        }
    }
}
