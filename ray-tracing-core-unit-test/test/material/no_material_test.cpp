#include "no_material_test.h"
#include "cute.h"
#include <material/no_material.h>
#include <math/ray.h>
#include <core/hit_record.h>
#include <core/scatter_record.h>
#include <test_assert_helper.h>

namespace material
{
	using namespace ray_tracing_core::material;
	using namespace ray_tracing_core;

	void no_material_new_material(void)
	{
		auto no_material = NoMaterial();
		auto ray = math::Ray::new_ray(math::Point3D(), math::Vector3D());
		core::HitRecord hit_record;
		core::ScatterRecord scatter_record;
		ASSERT_EQUAL(false, no_material.scatter(ray, hit_record, scatter_record));
		ASSERT_EQUAL(1, no_material.scattering_pdf(ray, hit_record, ray));
		assert_equal_color(core::Color(0), no_material.emitt(ray, hit_record), 0.001f);
		ASSERT_EQUAL(true, no_material.has_alpha());
	}
}
