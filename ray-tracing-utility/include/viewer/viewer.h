#ifndef __RTU__VIEWER__VIEWER_H__
#define __RTU__VIEWER__VIEWER_H__

#include "renderer/renderer.h"
#include <functional>

namespace ray_tracing_utility
{
	namespace viewer
	{
		class Viewer
		{
		protected:
			std::function<void(const renderer::Renderer&, bool)> image_store_callback;

		public:
			template<typename T>
			inline Viewer& set_image_store_callback(T callback);
			virtual void preview(const renderer::Renderer &renderer) = 0;
		};

		template<typename T>
		Viewer& Viewer::set_image_store_callback(T callback)
		{
			image_store_callback = callback;
			return *this;
		}
	}
}

#endif