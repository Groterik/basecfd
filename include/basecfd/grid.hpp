#ifndef BASECFD_GRID_GUARD_H
#define BASECFD_GRID_GUARD_H

#include <basecfd/definitions.hpp>
#include <basecfd/cell.hpp>
#include <basecfd/vector.hpp>

namespace basecfd
{

class Grid
{
public:
    typedef Cell<Grid> GridCell;

    virtual idx_t nc() const = 0;
    virtual idx_t np() const = 0;
    virtual Vector p(idx_t pos, int n) const = 0;
    virtual idx_t np(idx_t pos) const = 0;
    virtual double f(idx_t pos, dim_t d) const = 0;
    virtual int r(idx_t pos) const = 0;
    virtual dim_t nn(idx_t pos) const = 0;
    virtual idx_t neig(idx_t pos, dim_t n) const = 0;
    virtual dim_t nf() const = 0;

private:

    virtual void checkConsistency();
    virtual void scatter();
    virtual void replenishBoundary();
    virtual void reorderNeighbors();
    virtual void reorderPoints();
    virtual void sortCells();
};

template <class T>
class GridWrapper : public Grid
{
public:
    GridWrapper(T* g) : g(g) {}

    virtual idx_t nc() const
    {
        return g->nc();
    }
    virtual idx_t np() const
    {
        return g->np();
    }
private:
    virtual Vector p(idx_t pos, int n) const
    {
        return g->p(pos, n);
    }
    virtual idx_t np(idx_t pos) const
    {
        return g->np(pos);
    }
    virtual double f(idx_t pos, dim_t d) const
    {
        return g->f(pos, d);
    }
    virtual int r(idx_t pos) const
    {
        return g->r(pos);
    }
    virtual dim_t nn(idx_t pos) const
    {
        return g->nn(pos);
    }
    virtual idx_t neig(idx_t pos, dim_t n) const
    {
        return g->neig(pos, n);
    }
    virtual dim_t nf() const
    {
        return g->nf();
    }
    T* g;
};

} // namespace basecfd

#endif //BASECFD_GRID_GUARD_H
