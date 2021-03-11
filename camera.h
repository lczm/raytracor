#pragma once

#include "rt.h"

class camera {
    public:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;

        vec3 u;
        vec3 v;
        vec3 w;

        double lens_radius;

    public:
        camera(point3 lookfrom,
               point3 lookat,
               vec3 vup,
               double vfov, // vertical field-of-view in degrees
               double aspect_ratio,
               double aperture,
               double focus_dist) {
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);

            // height of the viewport is 2.0 units (-1 to 1)
            auto viewport_height = 2.0 * h;
            // height of the viewport width follows the aspect ratio
            auto viewport_width = aspect_ratio * viewport_height;

            // distance between projection plane and projection point
            auto focal_length = 1.0;

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            origin = lookfrom;
            // x-axis range : 0 to viewport_width
            horizontal = focus_dist * viewport_width * u;
            // y-axis range : 0 to viewport_height
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal / 2
                                       - vertical / 2
                                       - focus_dist * w;
            lens_radius = aperture / 2;
        }

        ray get_ray(double s, double t) const {
            vec3 rd = lens_radius * random_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();

            return ray(
                    origin + offset,
                    lower_left_corner + s * horizontal + t * vertical - origin - offset
                    );
        }
};
