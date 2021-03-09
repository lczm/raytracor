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

    public:
        metal(const color& a): albedo(a) {}

        virtual bool scatter(
                const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
            ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
};
