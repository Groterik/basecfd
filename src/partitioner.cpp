#include <basecfd/partitioner.hpp>

using namespace basecfd;

PartitionerFactory& PartitionerFactory::instance()
{
    static PartitionerFactory inst;
    return inst;
}

Partitioner& PartitionerFactory::partitioner()
{
    if (!impl)
    {
        throw std::runtime_error("no active partitioner");
    }
    return *impl;
}

void PartitionerFactory::partitioner(Partitioner *impl)
{
    this->impl = impl;
}

PartitionerFactory::PartitionerFactory()
    : impl(0)
{

}
