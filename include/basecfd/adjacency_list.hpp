#ifndef BASECFD_ADJACENCY_LIST_GUARD_H
#define BASECFD_ADJACENCY_LIST_GUARD_H

#include <basecfd/definitions.hpp>
#include <basecfd/range.hpp>

#include <vector>
#include <stdexcept>
#include <cassert>

namespace basecfd
{

template <class T1 = idx_t, class T2 = size_t, class VertexContext = void, class EdgeContext = void>
class Csr;


/**
 * Adjacency matrix representation
 *         V21 V22
 * V11      +   -
 * V12      -   -
 * V13      +   +
 *
 * V1 and V2 can have differrent nature (for example, cells and points)
 * V1 and V2 have to be presented by indices of type T1
 */
template <class T1, class T2>
class Csr<T1, T2, void, void>
{
    typedef std::vector<T2> PtrVec;
public:
    typedef T2 PtrType;
    typedef T1 IndType;
    typedef std::vector<IndType> IndVec;

    typedef typename IndVec::const_iterator const_neig_iterator;

    typedef Range<const_neig_iterator> NeigsRange;

    Csr()
    {
        col_ptr.push_back(0);
    }

    IndType add()
    {
        col_ptr.push_back(col_ptr.back());
        return static_cast<IndType>(col_ptr.size() - 2);
    }

    IndType add(const NeigsRange& neigs)
    {
        col_ptr.push_back(col_ptr.back() + neigs.size());
        row_ind.insert(row_ind.end(), neigs.begin(), neigs.end());
        return static_cast<IndType>(col_ptr.size() - 2);
    }

    size_t size() const
    {
        return col_ptr.size() - 1;
    }

    PtrType neigs() const
    {
        return col_ptr.back();
    }

    PtrType neigs(IndType pos) const
    {
        assert(pos < size());
        return col_ptr[pos + 1] - col_ptr[pos];
    }

    IndType neig(IndType pos, PtrType ind) const
    {
        assert(ind < neigs(pos));
        return row_ind[col_ptr[pos] + ind];
    }

    const_neig_iterator neigs_begin(IndType pos) const
    {
        assert(pos < size());
        return row_ind.begin() + col_ptr[pos];
    }

    const_neig_iterator neigs_end(IndType pos) const
    {
        assert(pos < size());
        return row_ind.begin() + col_ptr[pos + 1];
    }

    NeigsRange neigs_range(IndType pos) const
    {
        assert(pos < size());
        return Range<const_neig_iterator>(neigs_begin(pos), neigs_end(pos));
    }

    void link(IndType pos, IndType with)
    {
        assert(pos < size());
        row_ind.insert(row_ind.begin() + col_ptr[pos + 1], 1, with);
        const size_t s = col_ptr.size();
        for (size_t i = pos + 1; i < s; ++i)
        {
            ++col_ptr[i];
        }
    }

    void link(IndType pos, const NeigsRange& with)
    {
        assert(pos < size());
        row_ind.insert(row_ind.begin() + col_ptr[pos + 1], with.begin(), with.end());
        const size_t s = col_ptr.size();
        for (size_t i = pos + 1; i < s; ++i)
        {
            col_ptr[i] += with.size();
        }
    }

    void reorder(const IndVec& order)
    {
        if (order.size() != size())
        {
            throw std::runtime_error("invalid order size");
        }
        Csr tmp;
        size_t s = size();
        for (size_t ind = 0; ind < s; ++s)
        {
            tmp.add(neigs_range(ind));
        }
        swap(tmp);
    }

    void swap(Csr& a)
    {
        col_ptr.swap(a.col_ptr);
        row_ind.swap(a.row_ind);
    }

private:
    std::vector<PtrType> col_ptr;
    std::vector<IndType> row_ind;
};

} // namespace basecfd

#endif //BASECFD_ADJACENCY_LIST_GUARD_H
