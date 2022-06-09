#ifndef FLUX_MARCHING_TETS_H
#define FLUX_MARCHING_TETS_H

#include "grid.h"
#include "element.h"
#include "halfedges.h"
#include "tet-functions.h"

namespace flux {
class MarchingTet {
public:
MarchingTet(const std::vector<int>& sizes, TetFunction function);

private:
Grid<Tet>& _tet_grid;
TetFunction& _function;
HalfEdgeMesh<Triangle&> _result;


};


} // flux


#endif