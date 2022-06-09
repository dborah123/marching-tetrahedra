#ifndef FLUX_MARCHING_TETS_H
#define FLUX_MARCHING_TETS_H

#include "grid.h"
#include "element.h"
#include "halfedges.h"
#include "tet-functions.h"

namespace flux {
class MarchingTet {
public:
MarchingTet(Grid<Tet> tet_grid, TetFunction& function);

private:
Grid<Tet>& _tet_grid;
TetFunction& _function;
std::set<HalfEdge*> _inserted_edges;
HalfEdgeMesh<Triangle> _result;



};


} // flux


#endif