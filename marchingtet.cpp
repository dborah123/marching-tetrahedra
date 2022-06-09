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


} // flux