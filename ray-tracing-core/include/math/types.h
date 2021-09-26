#ifndef __RTC__MATH__TYPES_H__
#define __RTC__MATH__TYPES_H__

#include <tuple>

#define VECTOR_AND_MATRIX_LIBRARY GLM

#if VECTOR_AND_MATRIX_LIBRARY == GLMS

// GLM [https://glm.g-truc.net/0.9.9/api/index.html]
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ray_tracing_core
{
	namespace math
	{
		using Distance = double;
		using DistanceRange = std::tuple<Distance, Distance>;
		using Point3D = glm::dvec3;
		using Vector3D = glm::dvec3;
		using ColorRGB = glm::vec3;
		using Time = double;

		template<typename T>
		T minimum(T first_term, T second_term)
		{
			return glm::min(first_term, second_term);
		}

		template<typename T>
		T maximum(T first_term, T second_term)
		{
			return glm::max(first_term, second_term);
		}

		template<typename T>
		inline Distance dot(T first_vector, T second_vector)
		{
			return glm::dot(first_vector, second_vector);
		}
	}
}

#endif

#endif
