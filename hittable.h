#pragma once

#include "ray.h"

class material;

struct hit_record {
    point3 p;
    vec3 normal;
    // When a ray hits the surface, mata_ptr will be set to the
    // material that the surface has
    shared_ptr<material> mat_ptr;
    double t;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};
