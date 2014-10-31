#ifndef BASECFD_ALGORITHM_GUARD_H
#define BASECFD_ALGORITHM_GUARD_H

#include <algorithm>

namespace basecfd
{

template <class TRange1, class TRange2>
bool is_permutation(const TRange1& range1, const TRange2& range2)
{
    return range1.size() == range2.size() ? std::is_permutation(range1.begin(), range1.end(), range2.begin()) : false;
}

} // namespace basecfd

#endif //BASECFD_ALGORITHM_GUARD_H
