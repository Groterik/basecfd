#include <basecfd/adjoiner.hpp>

#include <metis.h>

#ifdef BASECFD_WITH_UNIT_TESTS
#define CATCH_CONFIG_MAIN
#include <thirdparty/catch/catch.hpp>
#include <basecfd/algorithm.hpp>
#endif // BASECFD_WITH_UNIT_TESTS

class MetisAdjoiner : public basecfd::Adjoiner
{
public:
    virtual void join(const basecfd::Adjacency& adj, size_t p, size_t common, basecfd::Adjacency& out) BASECFD_OVERRIDE;
};


void MetisAdjoiner::join(const basecfd::Adjacency& adj, size_t p, size_t common, basecfd::Adjacency& out)
{
    std::vector<idx_t> eptr(adj.eptr().begin(), adj.eptr().end());
    std::vector<idx_t> eind(adj.eind().begin(), adj.eind().end());
    idx_t nc = static_cast<idx_t>(adj.size());
    idx_t np = static_cast<idx_t>(p);
    idx_t ncommon = static_cast<idx_t>(common);
    idx_t flags = 0;

    idx_t* adjcny = 0;
    idx_t* xadj = 0;
    int err = METIS_MeshToDual(&nc, &np, eptr.data(), eind.data(), &ncommon, &flags, &xadj, &adjcny);
    if (err != METIS_OK)
    {
        throw std::runtime_error("failed to complete METIS_MeshToDual with code = " + std::to_string(err));
    }
    basecfd::Adjacency::PtrVec outptr(xadj, xadj + eptr.size());
    basecfd::Adjacency::PtrVec outind(adjcny, adjcny + outptr.back());

    METIS_Free(xadj);
    METIS_Free(adjcny);

    out = basecfd::Adjacency(outptr, outind);
}


#ifdef BASECFD_WITH_UNIT_TESTS
TEST_CASE("Metis adjoiner", "[metis_adjoiner]")
{
    MetisAdjoiner adjoiner;
    basecfd::Adjacency cell2points;
    cell2points.add({0, 1, 5, 6});     // 0
    cell2points.add({1, 2, 7, 6});     // 1
    cell2points.add({11, 12, 6, 7});   // 2
    cell2points.add({12, 13, 17, 18}); // 3
    cell2points.add({13, 14, 19, 18}); // 4
    cell2points.add({13, 14, 8, 9});   // 5
    cell2points.add({3, 4, 8, 9});     // 6
    cell2points.add({2, 3, 7, 8});     // 7

    basecfd::Adjacency cell2cell;
    REQUIRE_NOTHROW(adjoiner.join(cell2points, 20, 2, cell2cell));

    typedef std::vector<basecfd::idx_t> IdVec;
    REQUIRE(is_permutation(cell2cell.neigs_range(0), IdVec{1}));
    REQUIRE(is_permutation(cell2cell.neigs_range(1), IdVec{0, 2, 7}));
    REQUIRE(is_permutation(cell2cell.neigs_range(2), IdVec{1}));
    REQUIRE(is_permutation(cell2cell.neigs_range(3), IdVec{4}));
    REQUIRE(is_permutation(cell2cell.neigs_range(4), IdVec{3, 5}));
    REQUIRE(is_permutation(cell2cell.neigs_range(5), IdVec{4, 6}));
    REQUIRE(is_permutation(cell2cell.neigs_range(6), IdVec{5, 7}));
    REQUIRE(is_permutation(cell2cell.neigs_range(7), IdVec{1, 6}));

}

#endif // BASECFD_WITH_UNIT_TESTS
