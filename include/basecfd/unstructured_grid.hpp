#ifndef BASECFD_UNSTRUCTURED_GRID_GUARD_H
#define BASECFD_UNSTRUCTURED_GRID_GUARD_H

#include <basecfd/definitions.hpp>
#include <basecfd/vector.hpp>
#include <basecfd/element.hpp>
#include <basecfd/cell.hpp>

#include <cassert>
#include <vector>

namespace basecfd
{

struct MinimalCsr
{
    std::vector<idx_t> eptr;
    std::vector<idx_t> eind;
    std::vector<Vector> mp;
    std::vector<int> pctx;
    std::vector<double> f;
    std::vector<int> r;
    std::vector<ElementType> types;
};

class UnstructuredGrid
{
public:
    typedef Cell<UnstructuredGrid> GridCell;

    explicit UnstructuredGrid(const MinimalCsr& minCsr);

    idx_t np() const
    {
        return csr.np();
    }

    idx_t nc() const
    {
        return csr.nc();
    }

private:
    class Csr
    {
        /// Format part: eptr, eind, mp, f, r, types
        std::vector<idx_t> eptr;          //(     nc+1    ,   1   )
        std::vector<idx_t> eind;          //( eptr.back() ,   1   ) or (  nc  , elem.points() )
        std::vector<Vector> mp;           //(      np     ,  dim  )
        std::vector<Vector> oldmp;        //(      np     ,  dim  )
        std::vector<int> pctx;            //(      np     ,  dim  )
        std::vector<idx_t> xadj;          //(     nc+1    ,   1   )
        std::vector<idx_t> adjncy;        //( xadj.back() ,   1   ) or (  nc  ,  elem.neigs() )
        std::vector<double> f;            //(      nc     , ncomp )
        std::vector<int> r;               //(      nc     ,   1   )
        std::vector<ElementType> types;   //(      nc     ,   1   )

        idx_t nc() const
        {
            assert(!eptr.empty());
            return eptr.size() - 1;
        }

        idx_t np() const
        {
            assert(!eptr.empty());
            return eptr.back();
        }

        void CheckConsistency() const;
        void CheckMinimalConsistency() const;
        void LinkNeigs() const;
        friend class UnstructuredGrid;
    };

    Vector p(idx_t pos, int n) const
    {
        assert(n < np(pos));
        idx_t ip = csr.eptr[pos];
        return csr.mp[ip + n];
    }

    idx_t np(idx_t pos) const
    {
        assert(pos + 1 < csr.eptr.size());
        return csr.eptr[pos + 1] - csr.eptr[pos];
    }

    double f(idx_t pos, dim_t d) const
    {
        assert(pos * nComponents + d < csr.f.size());
        return csr.f[pos * nComponents + d];
    }

    int r(idx_t pos) const
    {
        return csr.r[pos];
    }

    dim_t nn(idx_t pos) const
    {
        assert(pos + 1 < csr.xadj.size());
        assert(csr.xadj[pos + 1] > csr.xadj[pos]);
        return static_cast<dim_t>(csr.xadj[pos + 1] - csr.xadj[pos]);
    }

    idx_t neig(idx_t pos, dim_t n) const
    {
        assert(n < nn(pos));
        idx_t in = csr.xadj[pos];
        return csr.adjncy[in + n];
    }

    dim_t nf() const
    {
        return nComponents;
    }

    Csr csr;
    dim_t nComponents;
};

} //namespace basecfd

#endif //BASECFD_UNSTRUCTURED_GRID_GUARD_H
