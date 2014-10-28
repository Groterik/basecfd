#include <basecfd/adjoiner.hpp>

#include <metis.h>

#ifdef BASECFD_WITH_UNIT_TESTS
#define CATCH_CONFIG_MAIN
#include <thirdparty/catch/catch.hpp>
#include <sstream>
#endif // BASECFD_WITH_UNIT_TESTS

class MetisAdjoiner : public basecfd::Adjoiner
{
public:
    virtual void join(const basecfd::Adjacency& adj, size_t p, size_t common, basecfd::Adjacency& out);
};


void MetisAdjoiner::join(const basecfd::Adjacency& adj, size_t p, size_t common, basecfd::Adjacency& out)
{
    std::vector<idx_t> eptr(adj.eptr().begin(), adj.eptr().end());
    std::vector<idx_t> eind(adj.eind().begin(), adj.eind().end());
    idx_t nc = static_cast<idx_t>(adj.size());
    idx_t np = static_cast<idx_t>(p);
    idx_t ncommon = static_cast<idx_t>(common);
    idx_t flags = 0;

    idx_t** adjcny = 0;
    idx_t** xadj = 0;
    int err = METIS_MeshToDual(&nc, &nc, eptr.data(), eind.data(), &ncommon, &flags, xadj, adjcny);
    if (err != METIS_OK)
    {
        throw std::runtime_error("failed to complete METIS_MeshToDual");
    }
    basecfd::Adjacency::PtrVec outptr(*xadj, *xadj + eptr.size());
    basecfd::Adjacency::PtrVec outind(*adjcny, *adjcny + outptr.back());

    METIS_Free(xadj);
    METIS_Free(adjcny);

    out = basecfd::Adjacency(outptr, outind);
}
