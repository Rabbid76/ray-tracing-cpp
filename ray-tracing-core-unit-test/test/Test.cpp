#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

#include "../test/ray_tracing_unit_test.h"

bool runSuite(int argc, char const *argv[]) {
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	cute::suite s = make_suite_ray_tracing_unit_test();
	bool success = runner(s, "ray_tracing_unit_test");
	return success;
}

int main(int argc, char const *argv[]) {
    return runSuite(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
