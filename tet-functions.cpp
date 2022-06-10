#include "tet-functions.h"
#include "vec.hpp"

namespace flux {

CircleTetFunction::CircleTetFunction(double radius) : _radius(radius) { }

double
CircleTetFunction::operator()(vec3d vertex) const {
    /**
     * Returns distance from sphere to vertex. Negative if the point is within the
     * sphere and positive if outside of in.
     */
    double mag = norm(vertex);
    double dist_to_sphere = mag - _radius;
    
    return dist_to_sphere;
}

} // flux