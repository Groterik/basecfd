#ifndef BASECFD_UNSTRUCTURED_GRID_GUARD_H
#define BASECFD_UNSTRUCTURED_GRID_GUARD_H

#include <basecfd/definitions.hpp>
#include <basecfd/vector.hpp>
#include <basecfd/element.hpp>
#include <basecfd/cell.hpp>

#include <cassert>
#include <vector>

namespace basecfd
{

struct MinimalCsr
{
    std::vector<idx_t> eptr;
    std::vector<idx_t> eind;
    std::vector<Vector> mp;
    std::vector<int> pctx;
    std::vector<double> f;
    std::vector<int> r;
    std::vector<ElementType> types;
};

namespace internal
{
class CsrHolder;
} // namespace internal

class UnstructuredGrid
{
public:
    typedef Cell<UnstructuredGrid> GridCell;

    explicit UnstructuredGrid(const MinimalCsr& minCsr);

    idx_t np() const;

    idx_t nc() const;

    Vector p(idx_t pos, int n) const;

    idx_t np(idx_t pos) const;

    double f(idx_t pos, dim_t d) const;

    int r(idx_t pos) const;

    dim_t nn(idx_t pos) const;

    idx_t neig(idx_t pos, dim_t n) const;

    dim_t nf() const;

    ~UnstructuredGrid();

private:
    internal::CsrHolder* csr;
    dim_t nComponents;
};

} //namespace basecfd

#endif //BASECFD_UNSTRUCTURED_GRID_GUARD_H
