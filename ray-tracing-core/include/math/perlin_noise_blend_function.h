#ifndef __RTC__MATH__PERLIN_NOISE_BLEND_FUNCTION_H__
#define __RTC__MATH__PERLIN_NOISE_BLEND_FUNCTION_H__

#include "math/blend_function.h"
#include "math/perlin_noise.h"
#include <cmath>

namespace ray_tracing_core::math
{
	class PerlinNoiseBlendFunction
		: public BlendFunction
	{
	public:
		enum class Type
		{
			Default = 0,
			Turb = 1,
			SinX = 2,
			SinY = 3,
			SinZ = 4,
		};

	private:
		PerlinNoise perlin_noise;
		Type type;
		math::Vector3D scale;

	public:
		inline PerlinNoiseBlendFunction(Type type, const math::Vector3D& scale);
		inline math::AlphaValue evaluate(
			const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const override;
		inline bool depends_on_texture_coordinates(void) const override;
	};

	PerlinNoiseBlendFunction::PerlinNoiseBlendFunction(Type type, const math::Vector3D& scale)
		: type(type)
		, scale(scale)
	{}

	math::AlphaValue PerlinNoiseBlendFunction::evaluate(
		const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const
	{
		double noise_value;
		switch (type) 
		{
			default:
			case Type::Default: noise_value = perlin_noise.noise(position * scale); break;
			case Type::Turb: noise_value = perlin_noise.turb(position * scale, 7); break;
			case Type::SinX: noise_value = std::sin(scale.x * position.x + 10.0 * perlin_noise.turb(position * scale, 7)); break;
			case Type::SinY: noise_value = std::sin(scale.y * position.y + 10.0 * perlin_noise.turb(position * scale, 7)); break;
			case Type::SinZ: noise_value = std::sin(scale.z * position.z + 10.0 * perlin_noise.turb(position * scale, 7)); break;
		}
		return static_cast<math::AlphaValue>(noise_value);
	}

	bool PerlinNoiseBlendFunction::depends_on_texture_coordinates(void) const
	{
		return false;
	}
}

#endif