#ifndef __RTU__RENDER__RENDERER_H__
#define __RTU__RENDER__RENDERER_H__

#include "math/types.h"
#include "core/Scene.h"
#include <cstdint>
#include <tuple>
#include <vector>

namespace ray_tracing_utility
{
	namespace renderer
	{
		class Renderer
		{
		public:
			using BufferData = std::pair<ray_tracing_core::math::ColorRGB, uint32_t>;
			using Buffer = std::vector<BufferData>;

		protected:
			Buffer buffer;

		public:
			virtual ~Renderer() = default;
			const Buffer &render_buffer() const { return buffer; }
			virtual void render(ray_tracing_core::core::Scene& scene, std::tuple<uint32_t, uint32_t> size, uint32_t samples) = 0;
			virtual bool is_finished() const = 0;
			virtual double percentage_done() const = 0;
		};
	}
}

#endif