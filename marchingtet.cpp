#include "marchingtet.h"
#include "halfedges.h"

namespace flux {

MarchingTet::MarchingTet(Grid<Tet>& tet_grid, TetFunction& function): 
_tet_grid(tet_grid),
_function(function),
_mesh(3) { }

void
MarchingTet::marching_tets() {
    /**
     * Performs the harching tet algorithm using the specified grid and function
     */
    
    // Loop thru tets
    int num_tets = _tet_grid.nb();
    for (int i = 0; i < num_tets; ++i) {
        // Examine the tet and its edges
        reconstruct_tet_surface(i);
    }
}

void
MarchingTet::reconstruct_tet_surface(const int tet_index) {
    
    // Loop thru edges in tet
    for (int i = 0; i < 3; ++i) {
        for (int j = i+1; j < 4; ++j) {
            int vert_index[2] = {_tet_grid(tet_index, i), _tet_grid(tet_index, j)};

            // Determine if intersection
                // Check if calc has already been performed
        }
    }

    // Check cases adn determine what to do
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
    double f0 = _function(v0);
    double f1 = _function(v1);
    double d0 = abs(f0);
    double d1 = abs(f1);

    // Calculating w0 and w1
    double w0 = (d1) / (d0 + d1);
    double w1 = (d0) / (d0 + d1);

    vec3d intersection_point = (w0*v0) + (w1*v1);
    return intersection_point;
}
} // flux