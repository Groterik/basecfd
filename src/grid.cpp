#include <basecfd/grid.hpp>

using namespace basecfd;

void Grid::init()
{
    checkConsistency();
    supressUseless();
    replenishBoundary();
    paint();
    scatter();
    reorderPoints();
    reorderNeighbors();
    sortCells();
    synchronizeOrder();
    checkConsistency();
}

GridInfo Grid::calculateInfo()
{

}

void Grid::scatter()
{
    // TODO
}

void Grid::replenishBoundary()
{
    // TODO
}

void Grid::supressUseless()
{
    // TODO
}

void Grid::reorderNeighbors()
{
    // TODO
}

void Grid::reorderPoints()
{
    // TODO
}

void Grid::sortCells()
{
    // TODO
}

void Grid::paint()
{
    // TODO
}

void Grid::synchronizeOrder()
{
    // TODO
}
