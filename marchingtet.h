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

/* Utility Functions */
Mesh<Triangle>& get_mesh();
HalfEdgeMesh<Triangle> get_halfmesh();
void run_viewer();
void test_validity();

private:
Grid<Tet>& _tet_grid;
TetFunction& _function;
std::map<std::set<int>, double> _inserted_edges; // Maps two vertices(edge) to 
                                                 // intersection 
std::map<int,double> _isovalues;                 // Maps tet vertex index to isovalue
std::map<int, int> _zero_isovalue_points;
Mesh<Triangle> _mesh;

/* Static Tables */
std::map<int,int> _edgetable; // Edge case mapped to triangle creation case
std::map<int, std::vector<int>> _triangletable; // Triangle creation case to vertex pairs
                                                // (edges) to create new triangle vertex
                                                // at edge intersection

/* Initialization Functions */
void initialize_edgetable();
void initialize_triangletable();
void preprocess_isovalues();

void reconstruct_tet_surface(int tet_index);

vec3d get_intersection_point(int v0_index, int v1_index);
int determine_case(std::vector<double>& tet_isovalues);
void get_tet_isovalues(int tet_index, std::vector<double>& tet_isovalues);

/* Adding Elements to Mesh */
void create_triangles(int tet_index, std::vector<int>& triangles_to_create);
int add_vertex_to_mesh(int tet_index, int index0, int index1);

/* Orientating New Triangle */
int check_orientation(int *tri_indices);
void change_orientation(int *tri_indices);

/* Other Checks */
int check_zero_isovalues(int v0_index, int v1_index, int new_vertex_index);
int check_repeat_vertices(int *tri_indices);
};
} // flux
#endif