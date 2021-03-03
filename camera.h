#pragma once

#include "rt.h"

class camera {
    public:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;

    public:
        camera() {
            auto aspect_ratio = 16.0 / 9.0;
            // height of the viewport is 2.0 units (-1 to 1)
            auto viewport_height = 2.0;
            // height of the viewport width follows the aspect ratio
            auto viewport_width = aspect_ratio * viewport_height;
            // distance between projection plane and projection point
            auto focal_length = 1.0;

            // note : going "into" the scene, is going to negatively into the z-axis
            // camera origin
            origin = point3(0, 0, 0);
            // x-axis range : 0 to viewport_width
            horizontal = vec3(viewport_width, 0.0, 0.0);
            // y-axis range : 0 to viewport_height
            vertical = vec3(0, viewport_height, 0);
            lower_left_corner = origin - horizontal / 2
                                       - vertical / 2
                                       - vec3(0, 0, focal_length);
        }

        ray get_ray(double u, double v) const {
            return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
        }
};
