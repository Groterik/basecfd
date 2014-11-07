#include <basecfd/unstructured_grid.hpp>
#include <basecfd/adjacency_list.hpp>
#include <basecfd/adjoiner.hpp>

using namespace basecfd;

namespace basecfd
{
namespace internal
{
class CsrHolder
{
    /// Format part: eptr, eind, mp, f, r, types
    Adjacency cell2points;
    std::vector<Vector> mp;           //(      np     ,  dim  )
    std::vector<Vector> oldmp;        //(      np     ,  dim  )
    std::vector<double> fp;           //(      np     ,  dim  )
    Adjacency cell2cell;
    std::vector<double> fc;           //(      nc     , ncomp )
    std::vector<int> r;               //(      nc     ,   1   )
    std::vector<ElementType> types;   //(      nc     ,   1   )

    idx_t nc() const
    {
        return cell2points.size();
    }

    idx_t np() const
    {
        return mp.size();
    }

    void makeFromMinimal(const MinimalCsr& minCsr);
    void checkConsistency() const;
    void checkMinimalConsistency() const;
    void linkNeigs();
    friend class basecfd::UnstructuredGrid;
};
} // namespace internal
} // namespace basecfd

UnstructuredGrid::UnstructuredGrid(const MinimalCsr& minCsr)
    : csr(new internal::CsrHolder)
{
    csr->makeFromMinimal(minCsr);
    csr->checkMinimalConsistency();
    csr->linkNeigs();
    csr->checkConsistency();
}

idx_t UnstructuredGrid::np() const
{
    return csr->np();
}

idx_t UnstructuredGrid::nc() const
{
    return csr->nc();
}

Vector UnstructuredGrid::p(idx_t pos, int n) const
{
    assert(static_cast<idx_t>(n) < np(pos));
    idx_t ip = csr->cell2points.neig(pos, n);
    return csr->mp[ip];
}

idx_t UnstructuredGrid::np(idx_t pos) const
{
    return csr->cell2points.neigs(pos);
}

double UnstructuredGrid::fc(idx_t pos, dim_t d) const
{
    assert(pos * nComponents + d < csr->fc.size());
    return csr->fc[pos * nComponents + d];
}

int UnstructuredGrid::r(idx_t pos) const
{
    return csr->r[pos];
}

dim_t UnstructuredGrid::nn(idx_t pos) const
{
    return static_cast<dim_t>(csr->cell2cell.neigs(pos));
}

idx_t UnstructuredGrid::neig(idx_t pos, dim_t n) const
{
    assert(n < nn(pos));
    idx_t in = csr->cell2cell.neig(pos, n);
    return in;
}

dim_t UnstructuredGrid::nfc() const
{
    return nComponents;
}

CellInfo UnstructuredGrid::ci(idx_t pos) const
{
    return CellInfo();
}

void UnstructuredGrid::checkConsistency() const
{
    csr->checkConsistency();
}

UnstructuredGrid::~UnstructuredGrid()
{
    delete csr;
}

UnstructuredGrid::GridCellRange UnstructuredGrid::range()
{
    return make_range(GridCellIterator(*this, 0), GridCellIterator(*this, nc()));
}

UnstructuredGrid::GridCellConstRange UnstructuredGrid::range() const
{
    return make_range(GridCellConstIterator(*this, 0), GridCellConstIterator(*this, nc()));
}

void UnstructuredGrid::scatter()
{
    // TODO
}

void UnstructuredGrid::replenishBoundary()
{
    // TODO
}

void UnstructuredGrid::supressUseless()
{
    // TODO
}

void UnstructuredGrid::reorderNeighbors()
{
    // TODO
}

void UnstructuredGrid::reorderPoints()
{
    // TODO
}

void UnstructuredGrid::sortCells()
{
    // TODO
}

void UnstructuredGrid::paint()
{
    // TODO
}

void UnstructuredGrid::synchronizeOrder()
{
    // TODO
}

void internal::CsrHolder::makeFromMinimal(const MinimalCsr& minCsr)
{
    cell2points = minCsr.cell2points;
    mp = minCsr.mp;
    fp = minCsr.fp;
    fc = minCsr.fc;
    r = minCsr.r;
    types = minCsr.types;
}

void internal::CsrHolder::checkConsistency() const
{
    checkMinimalConsistency();
    if (cell2cell.size() != cell2points.size())
    {
        throw std::runtime_error("invalid adjancency size");
    }
}

void internal::CsrHolder::checkMinimalConsistency() const
{
    const size_t nc = cell2points.size();
    if (nc == 0)
    {
        throw std::runtime_error("empty adjacency list (cell -> point)");
    }
    if (nc != r.size() || nc != types.size())
    {
        throw std::runtime_error("invalid minimal csr cell size");
    }
    if (fc.size() % nc != 0)
    {
        throw std::runtime_error("invalid F vector size for cells");
    }
    if (fp.size() % nc != 0)
    {
        throw std::runtime_error("invalid F vector size for points");
    }
}

void internal::CsrHolder::linkNeigs()
{
    AdjoinerFactory::instance().adjoiner().join(cell2points, mp.size(), 2, cell2cell);
}
