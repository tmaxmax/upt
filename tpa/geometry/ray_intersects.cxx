#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <optional>
#include <ranges>

// A three-dimensional vector determined by the origin and a given point.
struct Vec3 {
    double x, y, z;

    Vec3 operator+(Vec3 v) const noexcept {
        return {.x = x + v.x, .y = y + v.y, .z = z + v.z};
    }
    Vec3& operator+=(Vec3 v) noexcept { return *this = *this + v; }

    Vec3 operator-() const noexcept { return {.x = -x, .y = -y, .z = -z}; }
    Vec3 operator-(Vec3 v) const noexcept { return *this + (-v); }
    Vec3& operator-=(Vec3 v) noexcept { return *this = *this - v; }

    // Computes the dot product of the vectors.
    double operator*(Vec3 v) const noexcept {
        return x * v.x + y * v.y + z * v.z;
    }

    double length() const noexcept { return std::sqrt(*this * *this); }

    // Computes a point on the line determined by the vector.
    // Vectorial ecuation of a line: r = r0 + tv.
    // In our case, r0 = O and v is *this.
    Vec3 operator()(double t) const noexcept {
        return {.x = x * t, .y = y * t, .z = z * t};
    }
};

// A plane determined by a normal vector and a point contained within.
struct Plane {
    Vec3 p, n;

    // Returns the intersection of the plane with the line determined by the
    // given vector. If the line is included in the plane, it returns the input.
    std::optional<Vec3> intersection(Vec3 v) const noexcept {
        // We use the properties of the dot product first to determine whether
        // there is an intersection or not.
        // If the dot product of the normal vector with the vector to intersect
        // is 0, there are two scenarios that occur:
        //  1. the line determined by the vector to intersect is parallel
        //  2. it is included
        //
        // To determine which situation we are in, we use the dot product of the
        // normal vector and a vector determined by a point P in the plane and
        // a point V on the line of the vector to intersect – if this dot
        // product is 0, it means the vector is included, otherwise that it is
        // parallel.
        //
        // Given that our Vec3 is determined by the origin and another point,
        // we can consider the origin a point on the line of the vector. This
        // means that we already have the vector PV – it is simply p. Thus we
        // only need to compute the dot product of p and n, independently of v.
        // It's also important to note here that the orientation of the vector
        // is irrelevant, as we only compare the result to 0.
        //
        // As an optimization this dot product could be saved in a field of the
        // struct instance. For correct usage we'd need to add private fields
        // and a constructor so we skip this for simplicity.

        const auto nv = n * v;
        const auto np = n * p;

        constexpr auto zero_tolerance = 1e-6;

        if (std::abs(nv) < zero_tolerance) {
            if (std::abs(np) < zero_tolerance) {
                return v;
            }
            return {};
        }

        // This result is trivially obtained by substituting in the ecuation of
        // the plane the values of the intersection point coordinates based
        // on the vectorial ecuation of the line.
        const auto t = np / nv;

        // Use the solution in the ecuation of the line – this is our
        // intersection point.
        return v(t);
    }
};

std::istream& operator>>(std::istream& in, Vec3& v) {
    return in >> v.x >> v.y >> v.z;
}

// A rectangular cuboid determined by its bottom front left (start) and
// top back right (stop) corners.
struct BoundingBox {
    Vec3 start, stop; // We could have named these more explicitly but the code
                      // would've become uncomfortably long.

    std::array<Plane, 6> sides() const noexcept {
        // We can easily determine the planes of the sides using a normal vector
        // and a point because this is a box and the sides are perpendicular.

        const Vec3 bottom_left_back{.x = start.x, .y = start.y, .z = stop.z};
        const Vec3 bottom_right_front{.x = stop.x, .y = start.y, .z = start.z};
        const Vec3 top_left_front{.x = start.x, .y = stop.y, .z = start.z};
        return {
            Plane{.n = bottom_left_back - start, .p = start}, // front
            Plane{.n = bottom_right_front - start,
                .p = bottom_right_front},                            // right
            Plane{.n = top_left_front - start, .p = top_left_front}, // top
            Plane{.n = top_left_front - start, .p = start},          // bottom
            Plane{.n = bottom_right_front - start, .p = start},      // left
            Plane{.n = bottom_left_back - start, .p = bottom_left_back}, // back
        };
    }

    // Returns true if the determining point of the vector is on either
    // a side of the box or inside it.
    bool contains(Vec3 v) const noexcept {
        return start.x <= v.x && v.x <= stop.x && start.y <= v.y &&
               v.y <= stop.y && start.z <= v.z && v.z <= stop.z;
    }
};

// The idea: find the intersection points of the line determined by the ray
// with each side of the bounding box, keep only the points which are actually
// on the surface of the bounding box and select the point which together with
// the given origin determines the vector of minimum magnitude.

int main() {
    Vec3 origin, ray;
    std::cin >> origin >> ray;

    BoundingBox bb;
    std::cin >> bb.start >> bb.stop;

    // Use (0, 0, 0) as the origin to simplify the problem.
    ray -= origin;
    bb.start -= origin;
    bb.stop -= origin;

    namespace vw = std::views;
    constexpr auto inf = std::numeric_limits<double>::infinity();

    // Finite if it intersects, infinite otherwise.
    const auto distance =
        std::ranges::min(bb.sides() | vw::transform([&](auto p) {
            const auto v = p.intersection(ray);
            // By the definition of BoundingBox::contains, v,
            // which is contained by the plane of a side, when
            // also contained by the box it must be on its
            // surface.
            return (v && bb.contains(*v) ? v->length() : inf);
        }));

    std::cout << distance << '\n';
}