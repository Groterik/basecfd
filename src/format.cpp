#include <basecfd/format.hpp>

using namespace basecfd;

FormatFactory::FormatFactory()
{

}

FormatFactory& FormatFactory::instance()
{
    static FormatFactory inst;
    return inst;
}
