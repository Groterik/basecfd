#ifndef BASECFD_ADJOINER_GUARD_H
#define BASECFD_ADJOINER_GUARD_H

#include <basecfd/adjacency_list.hpp>

namespace basecfd
{

class Adjoiner
{
public:
    virtual void join(const Adjacency& adj, size_t p, size_t common, Adjacency& out) = 0;
    virtual ~Adjoiner() {}
};

class AdjoinerFactory
{
public:
    static AdjoinerFactory& instance();
    Adjoiner& adjoiner();
    void adjoiner(Adjoiner* impl);
private:
    AdjoinerFactory();
    Adjoiner* impl;
};

} // namespace basecfd

#endif //BASECFD_ELEMENT_GUARD_H
