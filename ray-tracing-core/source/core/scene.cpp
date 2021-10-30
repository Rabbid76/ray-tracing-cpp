#include <core/scene.h>
#include <material/material.h>
#include <limits>

namespace ray_tracing_core
{
    namespace core
    {
        Scene::Scene(const Configuration& configuration, const Camera& camera, const environment::Sky& sky, const ShapeNode& world)
            : configuration(configuration)
            , camera(camera)
            , sky(sky)
            , world(world)
        {}

        Color Scene::ray_trace_color(double u, double v) const
        {
            Color color(0);
            auto ray = camera.ray_to(u, v);
            Color attenuation(1.0f);
            for (int i = 0; i < configuration.maximum_depth; ++i)
            {
                HitRecord hit_record;
                if (world.hit(ray, { 0.001, std::numeric_limits<math::Distance>::max() }, hit_record))
                {
                    auto emitted = hit_record.material->emitt(ray, hit_record);
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
                        /*
                        let pdf: Option<Arc<dyn ProbabilityDensityFunction>> =
                            match scatter_record.pdf {
                                Some(pdf) => match light_shape {
                                    Some(ref light_shape) => Some(Arc::new(MixturePdf::new(
                                        pdf.clone(),
                                        Arc::new(HitAblePdf::new(
                                            &hit_record.position,
                                            light_shape.clone(),
                                        )),
                                    ))),
                                    None => Some(pdf.clone()),
                                },
                                None => None,
                            };
                            */
                            math::AlphaValue scattering_pdf;
                            if (pdf)
                            {
                                auto scattered = math::Ray::new_ray_with_attributes(hit_record.hit_point.position, pdf->generate(), ray);
                                scattering_pdf = 
                                    scatter_record.material->scattering_pdf(ray, hit_record, scattered) / pdf->value(scattered.direction);
                            }
                            else
                            {
                                scattering_pdf = scatter_record.material->scattering_pdf(ray, hit_record, scatter_record.ray);
                            }
                            attenuation *= scatter_record.attenuation * (math::AlphaValue)scattering_pdf;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    color += attenuation * sky.color_at(ray);
                    break;
                }
            }
            return color;
        }
    }
}