#ifndef BASECFD_CELL_GUARD_H
#define BASECFD_CELL_GUARD_H

#include <basecfd/definitions.hpp>
#include <basecfd/vector.hpp>

namespace basecfd
{

template <class G>
class Cell
{
public:
    typedef G GridType;
    Vector p(int n) const
    {
        return grid.g(pos, n);
    }

    dim_t np() const
    {
        return grid.np(pos);
    }

    double f(dim_t c) const
    {
        return grid.f(pos, c);
    }

    dim_t nf() const
    {
        return grid.nf();
    }

    int r() const
    {
        return grid.r(pos);
    }

    dim_t nn() const
    {
        return grid.nn(pos);
    }

    Cell neig(dim_t n) const
    {
        return Cell(grid, grid.neig(pos, n));
    }
private:
    Cell(G& grid, idx_t pos) : grid(grid), pos(pos) {}
    idx_t pos;
    G& grid;
};

} // namespace basecfd

#endif //BASECFD_CELL_GUARD_H
