#include "element.h"
#include "grid.h"
#include "webgl.h"
#include "marchingtet.h"
#include "tet-functions.h"

using namespace flux;

int
main(int argc, char *argv[]) {
    Grid<Tet> tet_grid({10,10,10});
    CircleTetFunction function(0.5);
    MarchingTet m_tet(tet_grid, function);
    m_tet.marching_tets();
    m_tet.run_viewer();
}