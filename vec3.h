#pragma once

#include <cmath>
#include <iostream>

using std::sqrt;

// Uses doubles but floats are fine either as well
class vec3 {
    // Attributes
    public:
        double e[3];

    // Methods
    public:
        vec3() : e{0, 0, 0} {}
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        double x() const {return e[0];}
        double y() const {return e[1];}
        double z() const {return e[2];}

        // Index operator overloads
        double operator[] (int i) const {return e[i];}
        double& operator[] (int i) {return e[i];}

        // Operator overloads
        vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}
        vec3& operator+=(const vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }
        vec3& operator*=(const double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }
        vec3& operator/=(const double t) {
            return *this *= 1/t;
        }

        // Utility methods
        double length() const { return sqrt(length_squared());}
        double length_squared() const {
            return e[0] * e[0] +
                   e[1] * e[1] +
                   e[2] * e[2];
        }

        inline static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }

        inline static vec3 random(double min, double max) {
            return vec3(random_double(min, max),
                        random_double(min, max),
                        random_double(min, max));
        }

        bool near_zero() const {
            // Return true if the vector is close to zero in all dimenisions
            const auto s = 1e-8;
            return (fabs(e[0]) < s)
                && (fabs(e[1]) < s)
                && (fabs(e[2]) < s);
        }
};

using point3 = vec3;
using color = vec3;

// General utility functions
static inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

static inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(
            u.e[0] + v.e[0],
            u.e[1] + v.e[1],
            u.e[2] + v.e[2]);
}

static inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(
            u.e[0] - v.e[0],
            u.e[1] - v.e[1],
            u.e[2] - v.e[2]);
}

static inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(
            u.e[0] * v.e[0],
            u.e[1] * v.e[1],
            u.e[2] * v.e[2]);
}

static inline vec3 operator*(double t, const vec3& v) {
    return vec3(
            t * v.e[0],
            t * v.e[1],
            t * v.e[2]);
}

static inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

static inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

static inline double dot(const vec3& u , const vec3& v) {
    return u.e[0] * v.e[0] +
           u.e[1] * v.e[1] +
           u.e[2] * v.e[2];
}

static inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

static inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

// Perhaps don't inline this? Function feels a little too big
// to inline
static vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() >= 1) {
            continue;
        }
        return p;
    }
}

static vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

static vec3 random_in_hemisphere(const vec3& normal) {
    vec3 in_unit_sphere = random_in_unit_sphere();
    // In the same hemisphere as the normal
    if (dot(in_unit_sphere, normal) > 0.0) {
        return in_unit_sphere;
    } else {
        return -in_unit_sphere;
    }
}

// Get the reflected ray
static vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

static vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    // uv and n are unit vectors, dot product to get cos_theta
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    // perpendicular -> r(perpendicular) = n/n^n * (r + cos_theta * n)
    // n/n^n =  (etai_over_etat)
    // r     = uv
    // n     = n
    // cos_theta calculated above
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    // Get the parallel by perpendicularing the perpendicular vector
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}
