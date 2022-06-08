#include "core/scene.h"
#include "material/material.h"
#include "math/random.h"
#include "pdf/geometry_pdf.h"
#include <limits>

namespace ray_tracing_core
{
    namespace core
    {
        Scene::Scene(const Configuration& configuration, const Camera& camera, const environment::Sky& sky, const ShapeNode& world)
            : configuration(configuration)
            , camera(&camera)
            , sky(&sky)
            , world(&world)
        {}

        Color Scene::ray_trace_color(double u, double v) const
        {
            Color color(0);
            auto ray = camera->ray_to(u, v);
            Color attenuation(1.0f);
            pdf::GeometryPDF geometry_pdf;
            if (lights.size() == 1)
                geometry_pdf.set_geometry(lights[0]);
            math::RandomGenerator generator;
            bool in_volume = false;
            for (uint32_t i = 0; i < configuration.maximum_depth; ++i)
            {
                HitRecord hit_record;
                hit_record.in_volume = in_volume;
                if (world->hit(ray, { 0.001, std::numeric_limits<math::Distance>::max() }, hit_record))
                {
                    hit_record.in_volume = false;
                    auto emitted = hit_record.material->emit(ray, hit_record);
                    color += attenuation * emitted;
                    ScatterRecord scatter_record;
                    if (hit_record.material->scatter(ray, hit_record, scatter_record))
                    {
                        if (scatter_record.is_specular)
                        {
                            attenuation *= scatter_record.attenuation;
                            ray = scatter_record.ray;
                        }
                        else 
                        {
                            auto pdf = scatter_record.probability_density_function.get();
                            if (!lights.empty() && generator.random_size() > 0.5)
                            {
                                if (lights.size() > 1)
                                {
                                    std::uniform_int_distribution<uint32_t> light_distribution(0, (uint32_t)(lights.size()-1));
                                    geometry_pdf.set_geometry(lights[light_distribution(generator.get_generator())]);
                                }
                                geometry_pdf.set_origin(hit_record.hit_point.position);
                                pdf = &geometry_pdf;
                            }
                            math::Distance scattering_pdf;
                            if (pdf)
                            {
                                auto scattered = math::Ray::new_ray_with_attributes(hit_record.hit_point.position, pdf->generate(), ray);
                                scattering_pdf = 
                                    scatter_record.material->scattering_pdf(ray, hit_record, scattered) / pdf->value(scattered.direction);
                                ray = scattered; // ? ray = scatter_record.ray;
                            }
                            else
                            {
                                scattering_pdf = scatter_record.material->scattering_pdf(ray, hit_record, scatter_record.ray);
                                ray = scatter_record.ray;
                            }
                            attenuation *= scatter_record.attenuation * (math::AlphaValue)scattering_pdf;
                        }
                        in_volume = hit_record.in_volume;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    color += attenuation * sky->color_at(ray);
                    break;
                }
            }
            return color;
        }
    }
}