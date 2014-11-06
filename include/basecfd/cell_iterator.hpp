#ifndef BASECFD_CELL_ITERATOR_GUARD_H
#define BASECFD_CELL_ITERATOR_GUARD_H

#include <basecfd/definitions.hpp>

#include <cassert>
#include <iterator>

namespace basecfd
{

template <class TGrid>
class CellIterator
    : public std::iterator<typename std::forward_iterator_tag, typename TGrid::CellType>
{
    typedef std::iterator<typename std::forward_iterator_tag, typename TGrid::CellType> Iterator;
public:
    typedef TGrid GridType;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::value_type value_type;
    // Default-constructible, copy-constructible, copy-assignable and destructible
    explicit CellIterator(GridType& grid, idx_t index = 0)
        : cell(grid, index) {}
    // Supports equality/inequality comparisons
    bool operator==(const CellIterator& rhs) const
    {
        assert(&(cell.grid()) == &(rhs.cell.grid()));
        return cell.index() == rhs.cell.index();
    }
    bool operator!=(const CellIterator& rhs) const
    {
        assert(&(cell.grid()) == &(rhs.cell.grid()));
        return cell.index() != rhs.cell.index();
    }

    // Can be dereferenced as an rvalue
    const reference operator*() const
    {
        return cell;
    }
    const pointer operator->() const
    {
        return &cell;
    }

    // Can be dereferenced as an lvalue
    reference operator*()
    {
        return cell;
    }
    pointer operator->()
    {
        return &cell;
    }

    // Can be incremented
    CellIterator& operator++()
    {
        cell = cell.next();
        return *this;
    }
    CellIterator operator++(int)
    {
        CellIterator tmp = *this;
        ++(*this);
        return tmp;
    }
private:
    value_type cell;
};


} // namespace basecfd

#endif //BASECFD_CELL_ITERATOR_GUARD_H
