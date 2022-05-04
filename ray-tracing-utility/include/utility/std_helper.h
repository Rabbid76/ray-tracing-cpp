#ifndef __RTU__CORE__STD_HELPER_H__
#define __RTU__CORE__STD_HELPER_H__

#include <sstream>
#include <string>

namespace ray_tracing_utility
{
	namespace utility
	{
		class formatter
		{
		public:
			std::stringstream message_stream;
			operator std::string() const { return message_stream.str(); }
			template <typename T>
			formatter& operator << (const T& value) { message_stream << value; return *this; }
		};
	}
}

#endif