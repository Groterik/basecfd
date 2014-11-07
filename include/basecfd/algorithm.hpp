#ifndef BASECFD_ALGORITHM_GUARD_H
#define BASECFD_ALGORITHM_GUARD_H

#include <basecfd/range.hpp>
#include <basecfd/filtered_iterator.hpp>

#include <algorithm>

namespace basecfd
{

template <class TRange1, class TRange2>
bool is_permutation(const TRange1& range1, const TRange2& range2)
{
    return range1.size() == range2.size() ? std::is_permutation(range1.begin(), range1.end(), range2.begin()) : false;
}

template <class TRange, class TUnaryPredicate>
Range<FilteredIterator<typename TRange::IteratorType, TUnaryPredicate> > filter(const TRange& range, TUnaryPredicate pred)
{
    return make_range(make_filtered_iterator(pred, range.begin(), range.end()),
                      make_filtered_iterator(pred, range.end(), range.end()));
}

template <class TRange, class TUnaryPredicate>
TUnaryPredicate for_each(const TRange& range, TUnaryPredicate pred)
{
    return std::for_each(range.begin(), range.end(), pred);
}


} // namespace basecfd

#endif //BASECFD_ALGORITHM_GUARD_H
