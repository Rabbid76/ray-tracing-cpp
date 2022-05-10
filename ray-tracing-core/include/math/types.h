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
        using ColorChannelRange = std::tuple<float, float>;
        using AlphaValue = float;
		using Time = double;
		using TimeRange = std::tuple<Time, Time>;
		using TextureDistance = float;
		using NormalMatrix = glm::dmat3;

		template<typename T> T minimum(T first_term, T second_term)
		{
			return glm::min(first_term, second_term);
		}

		template<typename T> T maximum(T first_term, T second_term)
		{
			return glm::max(first_term, second_term);
		}

		template<typename T> T normalize(const T &vector)
		{
			return glm::normalize(vector);
		}

		template<typename T> Distance length(const T& vector)
		{
			return glm::length(vector);
		}

		template<typename T> Distance dot(const T &first_vector, const T &second_vector)
		{
			return glm::dot(first_vector, second_vector);
		}

		template<typename T> T cross(const T &first_vector, const T &second_vector)
		{
			return glm::cross(first_vector, second_vector);
		}
	}
}

#endif

#endif
