#include <iostream>

#include "rt.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

color ray_color(const ray& r, const hittable& world, int depth) {
    // Create a temporary hit_record
    hit_record rec;

    // If exceeded ray bounce limit, don't generate anymore light
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    // If the world hits anything at the current ray, rec will be modified
    // with the appropriate values
    // Change 0 to 0.001 as t_min can be ~0.000000001 - shadow acne
    // https://digitalrune.github.io/DigitalRune-Documentation/html/3f4d959e-9c98-4a97-8d85-7a73c26145d7.htm
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;

        // Note : These are abstracted into materials
        // Uniform scatter direction???
        // point3 target = rec.p + random_in_hemisphere(rec.normal);
        // Lambertian diffuse
        // https://en.wikipedia.org/wiki/Lambertian_reflectance
        // point3 target = rec.p + rec.normal + random_unit_vector();
        // return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);

        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }

        return color(0, 0, 0);

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
    // Previously 100
    const int samples_per_pixel = 15;
    // So that it ray_color doesn't try to bounce limitlessly and segfault
    const int max_depth = 10;

    // World
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    // auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.0);
    // auto material_center = make_shared<dielectric>(1.5);
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.5);

    // "Ground" sphere - make it big enough to look like a ground of some sort
    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    // Center sphere
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    // Left sphere
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    // Another left sphere
    // If given a negative radius, it will become a glass sphere
    // Reflection will be similar to what it is like physically with a glass sphere
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
    // Right sphere
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

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
