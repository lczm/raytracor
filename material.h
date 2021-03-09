#pragma once

#include "rt.h"

struct hit_record;

// Abstract material class, for individual materials to inherit out of
class material {
    public:
        virtual bool scatter(
                const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
            ) const = 0;
};

// Lambertian
class lambertian : public material {
    public:
        color albedo;

    public:
        lambertian(const color& a): albedo(a) {}

        virtual bool scatter(
                const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
            ) const override {
            auto scatter_direction = rec.normal + random_unit_vector();

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
};

// Metal
class metal : public material {
    public:
        color albedo;
        double fuzz;

    public:
        // a = albedo, f = fuzz
        // fuzz parameter is to "blur" away the reflection
        // this can be made the most obvious if placed two metallic spheres
        // right next to a center sphere.
        // make one 0 fuzz and 1 fuzz respectively
        // results will be that the 0 fuzz sphere will be "clear" in reflection
        // and the 1 fuzz sphere will be "fuzz"-ed out in its reflection
        metal(const color& a, double f): albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
                const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
            ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            // scattered = ray(rec.p, reflected);
            scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
};

// Dielectric material -> water, glass, diamonds
// What makes a dielectric material "differentiable" between water/glass/diamonds
// is the index_of_refraction in the constructor
// list of indices : https://en.wikipedia.org/wiki/List_of_refractive_indices
// air     : ~1.0
// water   : ~1.33
// glass   : ~1.7
// diamond : ~2.4
class dielectric : public material {
    public:
        double ir;

    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(
                const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
            ) const override {
            attenuation = color(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

            vec3 unit_direction = unit_vector(r_in.direction());
            vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);
            scattered = ray(rec.p, refracted);
            return true;
        }
};
