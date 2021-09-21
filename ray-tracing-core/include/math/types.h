#ifndef __RTC__MATH__TYPES_H__
#define __RTC__MATH__TYPES_H__

#define VECTOR_AND_MATRIX_LIBRARY GLM

#if VECTOR_AND_MATRIX_LIBRARY == GLM

// GLM [https://glm.g-truc.net/0.9.9/api/index.html]
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ray_tracing_core
{
	namespace math
	{
		using Distance = double;
		using Point3D = glm::dvec3;
		using Vector3D = glm::dvec3;
		using Time = double;
	}
}

#endif

#endif
