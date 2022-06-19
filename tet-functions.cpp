#include "tet-functions.h"
#include "vec.hpp"

namespace flux {

double *
SphereTetFunction::get_center() {
    return _center;
}

SphereTetFunction::SphereTetFunction(double radius, double *center) :
_radius(radius)
{ 
    for (int i = 0; i < 3; ++i) 
        _center[i] = center[i];
}

double
SphereTetFunction::operator()(vec3d vertex) {
    /**
     * Returns distance from sphere to vertex. Negative if the point is within the
     * sphere and positive if outside of in.
     */
    // Getting center
    double *center = get_center();
    vec3d center_vec(center);

    vec3d difference = vertex - center_vec;
    double center_to_vertex = norm(difference);
    double dist_to_sphere = center_to_vertex - _radius;

    return dist_to_sphere;
}


} // flux