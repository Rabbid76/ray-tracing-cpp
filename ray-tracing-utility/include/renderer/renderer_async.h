#ifndef __RTU__RENDERER__RENDERER_ASYNC_H__
#define __RTU__RENDERER__RENDERER_ASYNC_H__

#include "renderer/renderer.h"
#include <future>
#include <vector>
#include <atomic>

namespace ray_tracing_utility
{
	namespace renderer
	{
		class RendererAsync
			: public Renderer
		{
		private:
			uint32_t number_of_threads;
			ray_tracing_core::core::Scene *render_scene;
			std::tuple<uint32_t, uint32_t> buffer_size;
			uint32_t render_samples{0};
			std::atomic<uint32_t> actual_sample_count{0};
			uint32_t render_fragments{ 0 };
			volatile uint32_t actual_render_fragments{ 0 };
			std::vector<std::future<void>> asyncs;

		public:
			RendererAsync(uint32_t number_of_threads);
			~RendererAsync() override;
			void render(ray_tracing_core::core::Scene& scene, std::tuple<uint32_t, uint32_t> size, uint32_t samples) override;
			bool is_finished() const override;
			double percentage_done() const override;

		private:
			void render();
		};
	}
}

#endif