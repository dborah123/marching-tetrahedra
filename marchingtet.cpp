#include "marchingtet.h"
#include "halfedges.h"
#include <cmath>

namespace flux {

MarchingTet::MarchingTet(Grid<Tet>& tet_grid, TetFunction& function): 
_tet_grid(tet_grid),
_function(function),
_mesh(3) {
    initialize_edgetable();
    initialize_triangletable();
}

void
MarchingTet::initialize_edgetable() {
    _edgetable[1]  = 13;
    _edgetable[14] = 13;

    _edgetable[2]  = 56;
    _edgetable[13] = 56;

    _edgetable[4]  = 38;
    _edgetable[11] = 38;

    _edgetable[8]  = 19;
    _edgetable[7]  = 19;

    _edgetable[3]  = 53;
    _edgetable[12] = 53;

    _edgetable[5]  = 43;
    _edgetable[10] = 43;

    _edgetable[6]  = 30;
    _edgetable[9]  = 30;
}

void
MarchingTet::initialize_triangletable() {
    /**
     * Initialize triangle table. This helps with determining the vertices that form 
     * the triangle(s)
     */
    _triangletable[13] = {0, 3, 4};
    _triangletable[56] = {3, 4, 5};
    _triangletable[38] = {1, 5, 4};
    _triangletable[19] = {0, 1, 2};

    _triangletable[53] = {0, 4, 2, 2, 4, 5};
    _triangletable[43] = {0, 1, 3, 1, 5, 3};
    _triangletable[30] = {1, 4, 2, 2, 4, 3};
}


void
MarchingTet::marching_tets() {
    /**
     * Performs the harching tet algorithm using the specified grid and function
     */
    // Perform preprocessing to get each vertex isovalue
    preprocess_isovalues();

    // Loop thru tets
    int num_tets = _tet_grid.nb();
    for (int i = 0; i < num_tets; ++i) {
        // Examine the tet and its edges
        reconstruct_tet_surface(i);
    }
}

void
MarchingTet::preprocess_isovalues() {
    /**
     * Calculates isovalues for vertices, storing them in _isovalue map
     */
    Vertices& vertices = _tet_grid.vertices();
    int num_vertices = vertices.nb();
    
    for (int i = 0; i < num_vertices; ++i) {
        _isovalues[i] = _function(vertices[i]);
    }
}

void
MarchingTet::reconstruct_tet_surface(int tet_index) {
    // Getting isovalues of specific tet
    std::vector<double> tet_isovalues;
    get_tet_isovalues(tet_index, tet_isovalues);

    // Getting the case we are in
    int tet_case = determine_case(tet_isovalues);
}

void
MarchingTet::get_tet_isovalues(int tet_index, std::vector<double>& tet_isovalues) {
    /**
     * Accumulates isovalues of specific tet into tet_isovalues vector
     */
    for (int i = 0; i < 4; ++i) {
        int vertex_index = _tet_grid(tet_index, i);
        double vertex_isovalue = _isovalues[vertex_index];
        tet_isovalues.push_back(vertex_isovalue);
    }
}

int
MarchingTet::determine_case(std::vector<double>& tet_isovalues) {
    /**
     * Returns case represented by the integer whose bits store which edges are 
     * intersected. Uses edgetable to assist in this
     * 
     * \returns: the case if one. If there is no intersection, then it returns 0
     */
    flux_assert(tet_isovalues.size() == 4);
    int res = 0;

    if (tet_isovalues[0] < 0) res != 1;
    if (tet_isovalues[1] < 0) res != 2;
    if (tet_isovalues[2] < 0) res != 4;
    if (tet_isovalues[3] < 0) res != 8;

    if (_edgetable.find(res) == _edgetable.end()) return 0;
    int tet_case = _edgetable[res];

    return tet_case;
}

vec3d
MarchingTet::get_intersection_point(int v0_index, int v1_index) {
    /**
     * Gets intersection point of edge with function
     */
    // Getting coordinates of vertices
    Vertices& vertices =_tet_grid.vertices();
    vec3d v0(vertices[v0_index]);
    vec3d v1(vertices[v1_index]);

    // Getting function values and abs'ing them
    // std::cout << "v0,v1: " << v0 << '\t' << v1 << std::endl;
    double f0 = _isovalues[v0_index];
    double f1 = _isovalues[v1_index];
    double d0 = abs(f0);
    double d1 = abs(f1);

    // Calculating w0 and w1
    double w0 = (d1) / (d0 + d1);
    double w1 = (d0) / (d0 + d1);

    for (int i = 0; i < 3; ++i) {
        v0[i] *= w0;
        v1[i] *= w1;
    }

    vec3d intersection_point = v0 + v1;

    return intersection_point;
}
} // flux