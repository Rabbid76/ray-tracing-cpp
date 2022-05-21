#ifndef __RTC__MATH__PERLIN_NOISE_H__
#define __RTC__MATH__PERLIN_NOISE_H__

#include "math/random.h"
#include "math/types.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

namespace ray_tracing_core::math
{
	class PerlinNoise
	{
	private:
		std::vector<Vector3D> noise_vectors;
		std::vector<uint8_t> noise_x;
		std::vector<uint8_t> noise_y;
		std::vector<uint8_t> noise_z;

	public:
		inline PerlinNoise();
		inline double noise(const Vector3D& p) const;
		inline double turb(const Vector3D& p, uint32_t depth) const;

	private:
		inline static std::vector<Vector3D> generate_vectors();
		inline static std::vector<uint8_t> generate_noise();
		inline static double trilinear_interpolation(const Vector3D c[2][2][2], double u, double v, double w);
	};

	PerlinNoise::PerlinNoise()
	{
		noise_vectors = generate_vectors();
		noise_x = generate_noise();
		noise_y = generate_noise();
		noise_z = generate_noise();
	}

	double PerlinNoise::noise(const Vector3D& p) const
	{
        auto i = (int)std::floor(p.x);
        auto j = (int)std::floor(p.y);
        auto k = (int)std::floor(p.z);
        Vector3D c[2][2][2];
        for (int di = 0; di < 2; ++di)
        {
            for (int dj = 0; dj < 2; ++dj)
            {
                for (int dk = 0; dk < 2; ++dk)
                {
                    c[di][dj][dk] = noise_vectors[
                        noise_x[(i + di) & 255] ^
                        noise_y[(j + dj) & 255] ^
                        noise_z[(k + dk) & 255]];
                }
            }
        }
        auto u = p.x - std::floor(p.x);
        auto v = p.y - std::floor(p.y);
        auto w = p.z - std::floor(p.z);
        return trilinear_interpolation(c, u, v, w);
	}

	double PerlinNoise::turb(const Vector3D& p, uint32_t depth) const
	{
		auto accum = 0.0;
		auto weight = 1.0;
		auto temp_p = p;
		for (uint32_t i = 0; i < depth; ++i)
		{
			accum += weight * noise(temp_p);
			weight *= 0.5;
			temp_p = temp_p * 2.0;
		}
		return std::abs(accum);
	}

	std::vector<Vector3D> PerlinNoise::generate_vectors()
	{
		RandomGenerator generator;
		std::vector<Vector3D> vectors(256);
		std::generate(vectors.begin(), vectors.end(), [&]() -> Vector3D
			{
				return generator.random_unit_vector();
			});
		return vectors;
	}

	std::vector<uint8_t> PerlinNoise::generate_noise()
	{
		RandomGenerator generator;
		std::vector<double> float_noise(256);
		std::generate(float_noise.begin(), float_noise.end(), [&]() -> double
			{
				return generator.random_size();
			});
		std::vector<uint8_t> noise(float_noise.size());
		for (size_t i = 0; i < noise.size(); ++i)
		{
			auto it = std::min_element(float_noise.begin(), float_noise.end());
			auto j = it - float_noise.begin();
			noise[i] = static_cast<uint8_t>(j);
			*it = 2.0f;
		}
		return noise;
	}

	double PerlinNoise::trilinear_interpolation(const Vector3D c[2][2][2], double u, double v, double w)
	{
        const auto uu = u * u * (3.0 - 2.0 * u);
        const auto vv = v * v * (3.0 - 2.0 * v);
        const auto ww = w * w * (3.0 - 2.0 * w);
        auto accum = 0.0;
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                for (int k = 0; k < 2; ++k)
                {
                    accum += dot(
                        Vector3D(u - i, v - j, w - k),
                        c[i][j][k]
                    ) * 
                    ((i * uu) + (1.0 - i) * (1.0 - uu)) * 
                    ((j * vv) + (1.0 - j) * (1.0 - vv)) *
                    ((k * ww) + (1.0 - k) * (1.0 - ww));
                }
            }
        }
        return accum;
	}
}

#endif