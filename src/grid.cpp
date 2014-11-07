#include <basecfd/grid.hpp>

#include <basecfd/algorithm.hpp>

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

void lowerUpperVector(Vector& minV, Vector& maxV, const Vector& a)
{
    for (dim_t d = 0; d < Vector::MAXDIM; ++d)
    {
        if (minV[d] > a[d])
        {
            minV[d] = a[d];
        }
        if (maxV[d] < a[d])
        {
            maxV[d] = a[d];
        }
    }
}

class InfoProcessor
{
public:
    InfoProcessor(GridInfo& info) : info(info) {}

    void process(const Grid::GridConstCell& cell)
    {
        info.add(cell.info());
        Vector ming, maxg;
        std::tie(ming, maxg) = info.envelopingSpace();
        Vector minv = cell.p(0);
        Vector maxv = cell.p(0);
        for (idx_t p = 1; p < cell.np(); ++p)
        {
            Vector v = cell.p(p);
            lowerUpperVector(minv, maxv, v);
            lowerUpperVector(ming, maxg, v);
        }
        info.envelopingSpace(GridInfo::Volume(ming, maxg));
        Vector d = maxv - minv;
        std::tie(minv, maxv) = info.cellOverallDimension();
        lowerUpperVector(maxv, minv, d);
        info.cellOverallDimension(GridInfo::Volume(minv, maxv));
    }

private:
    GridInfo& info;
};

GridInfo Grid::calculateInfo() const
{
    GridInfo result;
    InfoProcessor processor(result);
    for_each(range(), [&processor](const Grid::GridConstCell& cell) { processor.process(cell); });
    return result;
}

Grid::GridCellRange Grid::range()
{
    return make_range(GridCellIterator(*this, 0), GridCellIterator(*this, nc()));
}

Grid::GridCellConstRange Grid::range() const
{
    return make_range(GridCellConstIterator(*this, 0), GridCellConstIterator(*this, nc()));
}
