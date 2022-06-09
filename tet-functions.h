#ifndef FLUX_TET_FUNCTIONS_H
#define FLUX_TET_FUNCTIONS_H

#include "halfedges.h"
#include "vec.h"

namespace flux {

class TetFunction {
public:
    /**
     * \brief Returns the target size at a point x.
     */
    virtual double operator()( HalfVertex *vertex ) const = 0;

    /**
     * \brief virtual destructor.
     */
    virtual ~TetFunction() {}

};

class CircleTetFunction : public TetFunction {
public:
    CircleTetFunction(double radius);

    double operator()(HalfVertex *vertex) const;

private:
    double _radius;
};

}

#endif