#include "core_test.h"
#include "hit_record_test.h"

namespace core
{
	void add_core_unit_tests(cute::suite &suite)
	{
		suite.push_back(CUTE(hit_record_empty_test));
	}
}
