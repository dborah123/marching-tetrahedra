#ifndef FLUX_TET_FUNCTIONS_H
#define FLUX_TET_FUNCTIONS_H

#include "halfedges.h"
#include "vec.h"

namespace flux {

class TetFunction {
public:
    virtual double operator()( vec3d vertex ) = 0;
    virtual double *get_center() = 0;

    /**
     * \brief virtual destructor.
     */
    virtual ~TetFunction() {}

};

class SphereTetFunction : public TetFunction {
public:
    SphereTetFunction(double radius, double *center);

    double operator()(vec3d vertex);
    double *get_center();

private:
    double _radius;
    double _center[3];
};

}

#endif