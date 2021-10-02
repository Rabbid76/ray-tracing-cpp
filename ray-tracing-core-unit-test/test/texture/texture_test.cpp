#include "texture_test.h"
#include "constant_texture_test.h"

namespace texture
{
	void add_texture_unit_test(cute::suite& suite)
	{
		suite.push_back(CUTE(constant_texture_channels_test));
	}
}
