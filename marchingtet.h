#ifndef FLUX_MARCHING_TETS_H
#define FLUX_MARCHING_TETS_H

#include "grid.h"
#include "element.h"
#include "halfedges.h"
#include "tet-functions.h"
#include "mesh.h"

namespace flux {
class MarchingTet {
public:
MarchingTet(Grid<Tet>& tet_grid, TetFunction& function);
void marching_tets();

void reconstruct_tet_surface(const int tet_index);

/* Utility Functions */
Mesh<Triangle> get_mesh();
HalfEdgeMesh<Triangle> get_halfmesh();
void run_viewer();

private:
Grid<Tet>& _tet_grid;
TetFunction& _function;
std::set<std::set<int>> _inserted_edges;
Mesh<Triangle> _mesh;
// HalfEdgeMesh<Triangle> _result;



};


} // flux


#endif