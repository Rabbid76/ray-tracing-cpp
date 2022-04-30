#ifndef __RTU__VIEWER__VIEWER_CIMG_H__
#define __RTU__VIEWER__VIEWER_CIMG_H__

#include "viewer/viewer.h"

namespace ray_tracing_utility
{
	namespace viewer
	{
		class ViewerCImg
			: public Viewer
		{
		public:
			void preview(const renderer::Renderer &renderer) override;
		};
	}
}

#endif