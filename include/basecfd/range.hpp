#ifndef BASECFD_RANGE_GUARD_H
#define BASECFD_RANGE_GUARD_H

#include <cstddef>
#include <iterator>

namespace basecfd
{

template <class T>
class Range
{
public:
    typedef T IteratorType;
    typedef typename std::iterator_traits<IteratorType>::difference_type DifferenceType;

    Range(IteratorType begin, IteratorType end) : b(begin), e(end) {}

    template <class Y>
    Range(const Y& a) : b(a.begin()), e(a.end()) {}

    IteratorType begin() const
    {
        return b;
    }

    IteratorType end() const
    {
        return e;
    }

    bool empty() const
    {
        return b == e;
    }

    DifferenceType size() const
    {
        return std::distance(b, e);
    }

private:
    T b;
    T e;
};

template <class T>
inline Range<T> make_range(T begin, T end)
{
    return Range<T>(begin, end);
}

template <class T>
inline Range<T> make_range(const T& c)
{
    return make_range(c.begin(), c.end());
}

} // namespace basecfd

#endif //BASECFD_RANGE_GUARD_H
