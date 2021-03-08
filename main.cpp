#include <iostream>

#include "rt.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

color ray_color(const ray& r, const hittable& world, int depth) {
    // Create a temporary hit_record
    hit_record rec;

    // If exceeded ray bounce limit, don't generate anymore light
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    // If the world hits anything at the current ray, rec will be modified
    // with the appropriate values
    if (world.hit(r, 0, infinity, rec)) {
        point3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
    }

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
    const int image_width = 1280;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    // So that it ray_color doesn't try to bounce limitlessly and segfault
    const int max_depth = 10;

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera
    camera cam;

    // Render the image
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // Iterate over the amount of pixels (height)
    // Starting from image_height-1, as it starts from the top-left
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines Remaining: " << j << std::flush;
        // Iterate over the amount of pixels (width)
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            // Anti alias around the pixel
            // Using random is to "move" the pixel around a 0-1 area
            // Given enough times (samples_per_pixel), this will normalize out
            // and 'antialias' on the pixel.
            for (int s = 0; s < samples_per_pixel; ++s) {
                // Convert to relative scales
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                // Get a ray from the camera
                ray r = cam.get_ray(u, v);
                // Get the colour
                // Pass in max_depth first, as it will decrement as it recursively goes down
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}
