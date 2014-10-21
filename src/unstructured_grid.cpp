#include <basecfd/unstructured_grid.hpp>

using namespace basecfd;

UnstructuredGrid::UnstructuredGrid(const MinimalCsr& minCsr)
{
    csr.eind = minCsr.eind;
    csr.eptr = minCsr.eptr;
    csr.mp = minCsr.mp;
    csr.f = minCsr.f;
    csr.r = minCsr.r;
    csr.types = minCsr.types;

    csr.checkMinimalConsistency();
    csr.linkNeigs();
    csr.checkConsistency();
}


void UnstructuredGrid::Csr::checkConsistency() const
{
    // TODO
}

void UnstructuredGrid::Csr::checkMinimalConsistency() const
{
    // TODO
}

void UnstructuredGrid::Csr::linkNeigs() const
{
    // TODO
}
