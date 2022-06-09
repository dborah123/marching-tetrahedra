#include "element.h"
#include "grid.h"
#include "webgl.h"

using namespace flux;

int
main(int argc, char *argv[]) {
    Grid<Tet> tet_grid({5,5,5});
    Viewer viewer;
    viewer.add(tet_grid);
    viewer.run();
}