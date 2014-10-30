#ifndef BASECFD_PARTITIONER_GUARD_H
#define BASECFD_PARTITIONER_GUARD_H

#include <basecfd/adjacency_list.hpp>

namespace basecfd
{

class Partitioner
{
public:
    typedef size_t part_t;
    typedef std::vector<part_t> PartitionVec;
    virtual void partition(const Adjacency& adj, size_t p, size_t common, size_t parts, PartitionVec& output, PartitionVec& neigs) = 0;
    virtual ~Partitioner() {}
};

class PartitionerFactory
{
public:
    static PartitionerFactory& instance();
    Partitioner& partitioner();
    void partitioner(Partitioner* impl);
private:
    PartitionerFactory();
    Partitioner* impl;
};

} // namespace basecfd

#endif //BASECFD_ELEMENT_GUARD_H
