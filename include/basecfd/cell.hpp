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
        return m_grid->g(pos, n);
    }

    dim_t np() const
    {
        return m_grid->np(pos);
    }

    double f(dim_t c) const
    {
        return m_grid->f(pos, c);
    }

    dim_t nf() const
    {
        return m_grid->nf();
    }

    int r() const
    {
        return m_grid->r(pos);
    }

    dim_t nn() const
    {
        return m_grid->nn(pos);
    }

    Cell neig(dim_t n) const
    {
        return Cell(m_grid, m_grid->neig(pos, n));
    }

    Cell() : m_grid(0), pos(0) {}
    Cell(GridType& grid, idx_t pos) : m_grid(&grid), pos(pos) {}

    idx_t index() const
    {
        return pos;
    }

    const GridType& grid() const
    {
        return *m_grid;
    }

    GridType& grid()
    {
        return *m_grid;
    }

    Cell next() const
    {
        return Cell(*m_grid, pos + 1);
    }

private:
    GridType* m_grid;
    idx_t pos;
};

} // namespace basecfd

#endif //BASECFD_CELL_GUARD_H
