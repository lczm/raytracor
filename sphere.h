#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        point3 center;
        double radius;

    public:
        sphere() {}
        sphere(point3 cen, double r) : center(cen), radius(r) {};

        virtual bool hit(
                const ray& ray, double t_min, double t_max, hit_record& rec) const override;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 origin_center = r.origin() - center;
    // The 'squaring' part of the sphere equation - b^2
    auto a = r.direction().length_squared();
    // 2 * t * b - t = origin_center, b = r.direction()
    auto half_b = dot(origin_center, r.direction());
    // (a - c)^2 - r^2
    auto c = origin_center.length_squared() - radius * radius;
    // b^2 - 4ac
    auto discriminant = half_b * half_b - a * c;

    if (discriminant < 0)
        return false;

    auto sqrtd = sqrt(discriminant);
    
    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}
