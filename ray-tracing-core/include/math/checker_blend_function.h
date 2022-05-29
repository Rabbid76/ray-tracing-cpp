#ifndef __RTC__MATH__CHECKER_BLEND_FUNCTION_H__
#define __RTC__MATH__CHECKER_BLEND_FUNCTION_H__

#include "blend_function.h"

namespace ray_tracing_core::math
{
	class CheckerBlendFunction
		: public BlendFunction
	{
	private:
		math::Vector3D scale;

	public:
		inline CheckerBlendFunction(const math::Vector3D &scale);
		inline math::AlphaValue evaluate(
			const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const override;
		inline bool depends_on_texture_coordinates(void) const override;
	};

	CheckerBlendFunction::CheckerBlendFunction(const math::Vector3D &scale)
		: scale(scale)
	{}

	math::AlphaValue CheckerBlendFunction::evaluate(
		const core::TextureCoordinate& texture_coordinate, const math::Point3D& position) const
	{
		auto cosines = std::cos(scale.x * position.x) * std::cos(scale.y * position.y) * std::cos(scale.z * position.z);
		return cosines >= 0.0 ? 0.0f : 1.0f;
	}

	bool CheckerBlendFunction::depends_on_texture_coordinates(void) const
	{
		return false;
	}
}

#endif