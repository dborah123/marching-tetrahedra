#include "element.h"
#include "grid.h"
#include "webgl.h"
#include "marchingtet.h"
#include "tet-functions.h"

#include "halfedges.h"

using namespace flux;

int
main(int argc, char *argv[]) {
    Grid<Tet> tet_grid({10,10,10});

    // Defining function
    double center[3] = {0.5, 0.5, 0.5};
    SphereTetFunction function(0.40, center);

    // Creating marchingtet and performing operation
    MarchingTet m_tet(tet_grid, function);
    m_tet.marching_tets();
    Mesh<Triangle>& mesh = m_tet.get_mesh();
    
    HalfEdgeMesh<Triangle> halfmesh(mesh);
    m_tet.run_viewer();
}