#include "tet-functions.h"
#include "vec.hpp"

namespace flux {

CircleTetFunction::CircleTetFunction(double radius) :
_radius(radius)
{
    vec3d center({0.5, 0.5, 0.5});
    _center = center;
}

double
CircleTetFunction::operator()(vec3d vertex) const {
    /**
     * Returns distance from sphere to vertex. Negative if the point is within the
     * sphere and positive if outside of in.
     */
    vec3d difference = vertex - _center;
    double center_to_vertex = norm(difference);
    double dist_to_sphere = center_to_vertex - _radius;
    return dist_to_sphere;
}

} // flux