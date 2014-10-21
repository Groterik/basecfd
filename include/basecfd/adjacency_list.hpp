#ifndef BASECFD_ADJACENCY_LIST_GUARD_H
#define BASECFD_ADJACENCY_LIST_GUARD_H

#include <basecfd/definitions.hpp>

#include <vector>

namespace basecfd
{

template <class T1 = idx_t, class T2 = idx_t, class EdgeContext, class VertexContext>
class Csr
{
public:
    typedef T1 PtrType;
    typedef T2 IndType;
private:
    std::vector<PtrType> col_ptr;
    std::vector<IndType> row_ind;

};

} // namespace basecfd

#endif //BASECFD_ADJACENCY_LIST_GUARD_H
