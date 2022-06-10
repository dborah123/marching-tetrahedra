#ifndef FLUX_MARCHING_TETS_H
#define FLUX_MARCHING_TETS_H

#include "grid.h"
#include "element.h"
#include "halfedges.h"
#include "tet-functions.h"
#include "mesh.h"
#include <map>

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
std::map<std::set<int>, double> _inserted_edges;
Mesh<Triangle> _mesh;
// HalfEdgeMesh<Triangle> _result;


vec3d get_intersection_point(int v0_index, int v1_index);


};


} // flux


#endif