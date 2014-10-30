#ifndef BASECFD_RANGE_GUARD_H
#define BASECFD_RANGE_GUARD_H

#include <cstddef>

namespace basecfd
{


template <class T, class D = void>
class Range;

template <class T>
class Range<T, void>
{
public:
    typedef T Type;
    Range(T begin, T end) : b(begin), e(end) {}
    T begin() const
    {
        return b;
    }

    template <class Y>
    Range(const Y& a) : b(a.begin()), e(a.end()) {}

    T end() const
    {
        return e;
    }

    void begin(T value)
    {
        b = value;
    }

    void end(T value)
    {
        e = value;
    }

    size_t size() const
    {
        return e - b;
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
