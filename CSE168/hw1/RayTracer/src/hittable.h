#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct hit_record {
    point3 p;
    vec3 normal;
    double t;
    // design choice of determining the direction of normals at intersection of geometry time--normals always point "outward"; simply a matter of preference
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        // if the dot product is negative, they are going in opposing directions--if postive, they are going in similar direction
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif