#include <basecfd/adjoiner.hpp>

using namespace basecfd;

AdjoinerFactory& AdjoinerFactory::instance()
{
    static AdjoinerFactory inst;
    return inst;
}

Adjoiner& AdjoinerFactory::adjoiner()
{
    if (!impl)
    {
        throw std::runtime_error("no active adjoiner");
    }
    return *impl;
}

void AdjoinerFactory::adjoiner(Adjoiner* impl)
{
    this->impl = impl;
}

AdjoinerFactory::AdjoinerFactory()
    : impl(0)
{

}
