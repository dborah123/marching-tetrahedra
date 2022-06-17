#include "marchingtet.h"
#include "halfedges.h"
#include "webgl.h"
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
     * the triangle(s). It is important to note that edges are pairs of vertices, 
     * so 6 ints form a triangle
     */
    _triangletable[13] = {0,3, 0,1, 0,2};
    _triangletable[56] = {0,1, 3,1, 1,2};
    _triangletable[38] = {3,2, 1,2, 0,2};
    _triangletable[19] = {0,3, 2,3, 1,3};


    _triangletable[53] = {0,3, 1,3, 0,2, 0,2, 1,3, 1,2};
    _triangletable[43] = {0,3, 2,3, 0,1, 2,3, 1,2, 0,1};
    _triangletable[30] = {2,3, 1,3, 0,2, 0,2, 1,3, 0,1};

    // _triangletable[53] = {0, 4, 2, 2, 4, 5};
    // _triangletable[43] = {0, 1, 3, 1, 5, 3};
    // _triangletable[30] = {1, 4, 2, 2, 4, 3};
}

void
MarchingTet::marching_tets() {
    /**
     * Performs the harching tet algorithm using the specified grid and function
     */
    // Perform preprocessing to get each vertex's isovalue
    preprocess_isovalues();

    // Loop thru tets
    int num_tets = _tet_grid.nb();
    for (int i = 0; i < num_tets; ++i) {
        // Create triangle(s) in tet
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
    std::cout << "tet_isovalues: " << tet_isovalues[0] << ' ' << tet_isovalues[1] << ' ' << tet_isovalues[2] << ' '  << tet_isovalues[3] << std::endl;

    // Getting the case we are in. Returning if no intersections in tet
    int tet_case;
    if (!(tet_case = determine_case(tet_isovalues))) return;
    std::cout << tet_case << std::endl;

    std::vector<int> triangles_to_create = _triangletable[tet_case];
    create_triangles(tet_index, triangles_to_create);
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

    if (tet_isovalues[0] < 0) res |= 1;
    if (tet_isovalues[1] < 0) res |= 2;
    if (tet_isovalues[2] < 0) res |= 4;
    if (tet_isovalues[3] < 0) res |= 8;

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

void
MarchingTet::create_triangles(int tet_index, std::vector<int>& triangles_to_create) {
    /**
     * Create triangles in tet (denoted by tet_index). Triangle framework determined by
     * triangles_to_create
     */
    // Performing checks for size of triangles to create
    int size_ttc = triangles_to_create.size();
    flux_assert(size_ttc == 6 || size_ttc == 12);
    flux_assert(size_ttc % 6 == 0);

    // Add triangles to mesh
    int tri_indices[3];
    int mesh_index;
    for (int i = 0; i < size_ttc / 6; i++) { // Thru # of triangles (1 or 2)
        for (int j = 0; j < 6; j+=2) {
            // Getting indices from triangles_to_create
            int index0 = triangles_to_create[(j + (i*6))];
            int index1 = triangles_to_create[(j + (i*6) + 1)];

            // Translating index to _tet_grid vertex index and then creating new vertex
            mesh_index = add_vertex_to_mesh(tet_index, index0, index1);
            std::cout << "mesh index: " << mesh_index << '\t' << j/2 << std::endl;
            tri_indices[j/2] = mesh_index;
        }
        // Adding new triangle to mesh
        _mesh.add(tri_indices);
        std::cout << "New Triangle: " << tri_indices[0] << ' ' << tri_indices[1] << ' ' << tri_indices[2] << std::endl;
    }
    // return samson III
}

int
MarchingTet::add_vertex_to_mesh(int tet_index, int index0, int index1) {
    /**
     * PARAMS:
     * tet_index: the index that refers to the tet in _tet_grid
     * index0: the 0th vertex index relative to the tet
     * index1: the 1st vertex index relative to the tet
     * 
     * RETURNS:
     * index of the new vertex in _mesh
     */
    // Getting indices of vertices in _tet_grid
    int v0_index = _tet_grid(tet_index, index0);
    int v1_index = _tet_grid(tet_index, index1);

    std::set<int> vertices({v0_index, v1_index});

    // If point has already been created, return its vertex
    if (_inserted_edges.find(vertices) != _inserted_edges.end())
        return _inserted_edges[vertices];

    // Get intersection point and add new point to _mesh vertices
    vec3d new_point = get_intersection_point(v0_index, v1_index);
    std::cout << "new_point: " << new_point << "with indices: " << v0_index << ' ' << v1_index << std::endl;
    int new_vertex_index = _mesh.vertices().nb();
    _mesh.vertices().add(new_point.data());

    // Get new vertex's index, add to _inserted_edges and return
    _inserted_edges[vertices] = new_vertex_index;
    return new_vertex_index;
}


void
MarchingTet::run_viewer() {
    Viewer viewer;
    viewer.add(_mesh);
    viewer.run();
}
} // flux