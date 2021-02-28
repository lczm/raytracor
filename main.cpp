#include <iostream>

#include "color.h"
#include "ray.h"
#include "vec3.h"

bool hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 origin_center = r.origin() - center;
    // The 'squaring' part of the sphere equation - b^2
    auto a = dot(r.direction(), r.direction());
    // 2 * t * b - t = origin_center, b = r.direction()
    auto b = 2.0 * dot(origin_center, r.direction());
    // (a - c)^2 - r^2
    auto c = dot(origin_center, origin_center) - radius * radius;
    // b^2 - 4ac
    auto discriminant = b * b - 4 * a * c;
    // If there are 1 or 2 roots, it has an intersection.
    return (discriminant > 0);
}

color ray_color(const ray& r) {
    if (hit_sphere(point3(0, 0, -1), 0.5, r))
        return color(1, 0, 0);
    // Get unit vector of the ray
    vec3 unit_direction = unit_vector(r.direction());
    // unit_direction.y() as this is a gradient based on the y-axis
    auto t = 0.5 * (unit_direction.y() + 1.0);
    // When t = 1.0 - Get blue value
    // When t = 0.0 - Get white value
    // Linearly interpolate between the values through the height
    // If the value is between 0.0 and 1.0, then this will 'mix' the colors
    return (1.0 - t) * color(1.0, 1.0, 1.0)  // if 1.0, this negates the 'white' vlaue
            + t * color(0.5, 0.7, 1.0);      // this is the blue value
}

int main() {
    // Image dimensions, 16:9 aspect ratio, calculate width & height
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    // height of the viewport is 2.0 units
    auto viewport_height = 2.0;
    // height of the viewport width follows the aspect ratio
    auto viewport_width = aspect_ratio * viewport_height;
    // distance between projection plane and projection point
    auto focal_length = 1.0;

    // note : going "into" the scene, is going to negatively into the z-axis
    // camera origin
    auto origin = point3(0, 0, 0);
    // x-axis range : 0 to viewport_width
    auto horizontal = vec3(viewport_width, 0, 0);
    // y-axis range : 0 to viewport_height
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2
                             - vertical / 2
                             - vec3(0, 0, focal_length);

    // Render the image
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // Iterate over the amount of pixels (height)
    // Starting from image_height-1, as it starts from the top-left
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines Remaining: " << j << ' ' << std::flush;
        // Iterate over the amount of pixels (width)
        for (int i = 0; i < image_width; ++i) {
            // Relative scale of where the x-axis is on.
            auto u = double(i) / (image_width - 1);
            // Relative scale of where the y-axis is on.
            auto v = double(j) / (image_height - 1);
            ray r(origin, lower_left_corner 
                          + (u * horizontal)  // Multiply by horizontal viewport
                          + (v * vertical)    // Multiply by vertical viewport
                          - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}

