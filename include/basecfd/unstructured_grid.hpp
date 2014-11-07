#ifndef BASECFD_UNSTRUCTURED_GRID_GUARD_H
#define BASECFD_UNSTRUCTURED_GRID_GUARD_H

#include <basecfd/definitions.hpp>
#include <basecfd/adjacency_list.hpp>
#include <basecfd/vector.hpp>
#include <basecfd/element.hpp>
#include <basecfd/cell.hpp>
#include <basecfd/cell_info.hpp>
#include <basecfd/cell_iterator.hpp>

#include <cassert>
#include <vector>

namespace basecfd
{

struct MinimalCsr
{
    Adjacency cell2points;
    std::vector<Vector> mp;
    std::vector<double> fp;
    std::vector<double> fc;
    std::vector<int> r;
    std::vector<ElementType> types;
};

namespace internal
{
class CsrHolder;
} // namespace internal

class UnstructuredGrid
        : public CellIterable<UnstructuredGrid>
{
public:

    explicit UnstructuredGrid(const MinimalCsr& minCsr);

    idx_t np() const;

    idx_t nc() const;

    Vector p(idx_t pos, int n) const;

    idx_t np(idx_t pos) const;

    double fc(idx_t pos, dim_t d) const;

    int r(idx_t pos) const;

    dim_t nn(idx_t pos) const;

    idx_t neig(idx_t pos, dim_t n) const;

    dim_t nfc() const;

    CellInfo ci(idx_t pos) const;

    void checkConsistency() const;

    ~UnstructuredGrid();

    GridCellRange range();
    GridCellConstRange range() const;

    void scatter();
    void replenishBoundary();
    void supressUseless();
    void reorderNeighbors();
    void reorderPoints();
    void sortCells();
    void paint();
    void synchronizeOrder();

private:
    internal::CsrHolder* csr;
    dim_t nComponents;
};

} //namespace basecfd

#endif //BASECFD_UNSTRUCTURED_GRID_GUARD_H
