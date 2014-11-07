#ifndef BASECFD_GRID_GUARD_H
#define BASECFD_GRID_GUARD_H

#include <basecfd/definitions.hpp>
#include <basecfd/cell.hpp>
#include <basecfd/cell_info.hpp>
#include <basecfd/cell_iterator.hpp>
#include <basecfd/vector.hpp>
#include <basecfd/grid_info.hpp>

#include <memory>

namespace basecfd
{

class Grid
        : public CellIterable<Grid>
{
public:

    virtual idx_t nc() const = 0;
    virtual idx_t np() const = 0;
    virtual Vector p(idx_t pos, int n) const = 0;
    virtual idx_t np(idx_t pos) const = 0;
    virtual double fc(idx_t pos, dim_t d) const = 0;
    virtual int r(idx_t pos) const = 0;
    virtual dim_t nn(idx_t pos) const = 0;
    virtual idx_t neig(idx_t pos, dim_t n) const = 0;
    virtual dim_t nfc() const = 0;
    virtual CellInfo ci(idx_t pos) const = 0;

    void init();

    virtual GridInfo calculateInfo() const;
    virtual void checkConsistency() const = 0;

    GridCellRange range();
    GridCellConstRange range() const;

private:

    virtual void scatter() = 0;
    virtual void replenishBoundary() = 0;
    virtual void supressUseless() = 0;
    virtual void reorderNeighbors() = 0;
    virtual void reorderPoints() = 0;
    virtual void sortCells() = 0;
    virtual void paint() = 0;
    virtual void synchronizeOrder() = 0;
};

template <class T>
class GridWrapper : public Grid
{
public:
    GridWrapper(T* g) : g(g) {}

    virtual idx_t nc() const BASECFD_OVERRIDE
    {
        return g->nc();
    }
    virtual idx_t np() const BASECFD_OVERRIDE
    {
        return g->np();
    }
    virtual Vector p(idx_t pos, int n) const BASECFD_OVERRIDE
    {
        return g->p(pos, n);
    }
    virtual idx_t np(idx_t pos) const BASECFD_OVERRIDE
    {
        return g->np(pos);
    }
    virtual double fc(idx_t pos, dim_t d) const BASECFD_OVERRIDE
    {
        return g->fc(pos, d);
    }
    virtual int r(idx_t pos) const BASECFD_OVERRIDE
    {
        return g->r(pos);
    }
    virtual dim_t nn(idx_t pos) const BASECFD_OVERRIDE
    {
        return g->nn(pos);
    }
    virtual idx_t neig(idx_t pos, dim_t n) const BASECFD_OVERRIDE
    {
        return g->neig(pos, n);
    }
    virtual dim_t nfc() const BASECFD_OVERRIDE
    {
        return g->nfc();
    }

    virtual void checkConsistency() const BASECFD_OVERRIDE
    {
        return g->checkConsistency();
    }

    virtual CellInfo ci(idx_t pos) const BASECFD_OVERRIDE
    {
        return g->ci(pos);
    }

private:

    virtual void scatter() BASECFD_OVERRIDE
    {
        g->scatter();
    }

    virtual void replenishBoundary() BASECFD_OVERRIDE
    {
        g->replenishBoundary();
    }

    virtual void supressUseless() BASECFD_OVERRIDE
    {
        g->supressUseless();
    }

    virtual void reorderNeighbors() BASECFD_OVERRIDE
    {
        g->reorderNeighbors();
    }

    virtual void reorderPoints() BASECFD_OVERRIDE
    {
        g->reorderPoints();
    }

    virtual void sortCells() BASECFD_OVERRIDE
    {
        g->sortCells();
    }

    virtual void paint() BASECFD_OVERRIDE
    {
        g->paint();
    }

    virtual void synchronizeOrder() BASECFD_OVERRIDE
    {
        g->synchronizeOrder();
    }

    T* g;
};

} // namespace basecfd

#endif //BASECFD_GRID_GUARD_H
