#ifndef BASECFD_FILTERED_ITERATOR_GUARD_H
#define BASECFD_FILTERED_ITERATOR_GUARD_H

#include <algorithm>

namespace basecfd
{

template <class TIterator, class TUnaryPredicate>
class FilteredIterator
    : std::iterator<typename std::bidirectional_iterator_tag, typename TIterator::value_type>
{
    typedef std::iterator<typename std::bidirectional_iterator_tag, typename TIterator::value_type> Iterator;
public:
    typedef TUnaryPredicate UnaryPredicate;
    typedef TIterator IteratorType;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::value_type value_type;
    // Default-constructible, copy-constructible, copy-assignable and destructible
    FilteredIterator()
    {
        it = stop;
    }
    FilteredIterator(IteratorType it, IteratorType stop, UnaryPredicate pred) : it(it), stop(stop), pred(pred)
    {
        if (!pred(*it))
        {
            ++(*this);
        }
    }
    // Supports equality/inequality comparisons
    bool operator==(const FilteredIterator& rhs) const
    {
        return it == rhs.it;
    }
    bool operator!=(const FilteredIterator& rhs) const
    {
        return it != rhs.it;
    }

    // Can be dereferenced as an rvalue
    const reference operator*() const
    {
        return *it;
    }
    const pointer operator->() const
    {
        return it.operator->();
    }

    // Can be dereferenced as an lvalue
    reference operator*()
    {
        return *it;
    }
    pointer operator->()
    {
        return std::addressof(*it);
    }

    // Can be incremented
    FilteredIterator& operator++()
    {
        for (; it != stop; ++it)
        {
            if (pred(*it))
            {
                break;
            }
        }
        return *this;
    }
    FilteredIterator operator++(int)
    {
        FilteredIterator tmp = *this;
        ++(*this);
        return tmp;
    }

private:
    IteratorType it;
    IteratorType stop;
    UnaryPredicate pred;
};

template <class TIterator, class TUnaryPredicate>
FilteredIterator<TIterator, TUnaryPredicate> make_filtered_iterator(TUnaryPredicate pred, TIterator it, TIterator stop = TIterator())
{
    return FilteredIterator<TIterator, TUnaryPredicate>(it, stop, pred);
}

} // namespace basecfd


#endif //BASECFD_FILTERED_ITERATOR_GUARD_H
