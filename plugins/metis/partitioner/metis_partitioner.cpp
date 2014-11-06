#include <basecfd/partitioner.hpp>

#include <metis.h>

#ifdef BASECFD_WITH_UNIT_TESTS
#define CATCH_CONFIG_MAIN
#include <thirdparty/catch/catch.hpp>
#endif // BASECFD_WITH_UNIT_TESTS

class MetisPartitioner : public basecfd::Partitioner
{
public:
    virtual void partition(const basecfd::Adjacency& adj, size_t p, size_t common,
                           size_t parts, PartitionVec& output, PartitionVec& neigs) BASECFD_OVERRIDE;
};

void MetisPartitioner::partition(const basecfd::Adjacency& adj, size_t p, size_t common, size_t parts, PartitionVec& output, PartitionVec& neigs)
{
    idx_t nc = static_cast<idx_t>(adj.size());
    idx_t np = static_cast<idx_t>(p);
    idx_t ncommon = static_cast<idx_t>(common);
    idx_t nparts = static_cast<idx_t>(parts);
    std::vector<idx_t> eptr(adj.eptr().begin(), adj.eptr().end());
    std::vector<idx_t> eind(adj.eind().begin(), adj.eind().end());
    std::vector<idx_t> cpart(nc, 0);
    std::vector<idx_t> ppart(np, 0);
    idx_t opts = 0;
    int err = METIS_PartMeshDual(&nc, &np, eptr.data(), eind.data(), 0, 0, &ncommon, &nparts, 0, &opts, 0,
                       cpart.data(), ppart.data());
    if (err != METIS_OK) {
        throw std::runtime_error("failed to complete METIS_PartMeshDual with code = " + std::to_string(err));
    }
    output.assign(cpart.begin(), cpart.end());
    neigs.assign(ppart.begin(), ppart.end());

}
