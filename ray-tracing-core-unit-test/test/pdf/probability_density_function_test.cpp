#include "probability_density_function_test.h"
#include "cosine_pdf_test.h"

namespace pdf
{
	void add_probability_density_function_unit_test(cute::suite &suite)
	{
		suite.push_back(CUTE(cosine_pdf_test));
	}
}
