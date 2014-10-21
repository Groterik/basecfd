#ifndef BASECFD_FORMAT_GUARD_H
#define BASECFD_FORMAT_GUARD_H

#include <basecfd/definitions.hpp>
#include <basecfd/grid.hpp>

#include <map>
#include <string>
#include <vector>

namespace basecfd
{

enum FormatFlags
{
    NO_FLAGS = 0,
    BOUNDARY_COMPONENT = 1
};

class Formater
{
public:
    virtual Grid* load(const std::string& file, int flags) = 0;
    virtual void save(const Grid& grid, const std::string& file, int flags) = 0;
    virtual ~Formater() {}
};

class FormatFactory
{
public:
    static FormatFactory& instance();
    void registerExtension(const std::string& ext, Formater* formater);
    void unregisterExtension(const std::string& ext);
    Formater* getByExtension(const std::string& ext);
private:
    FormatFactory();
    typedef std::map<std::string, Formater*> FormatMap;
    FormatMap formats;
};

template <class T>
size_t squash(const std::vector<std::vector<T> >& in, std::vector<T>& out)
{
    if (in.empty())
    {
        return 0;
    }
    const size_t m = in.front().size();
    for (size_t i = 0; i < in.size(); ++i)
    {
        if (m != in[i].size())
        {
            return 0;
        }
    }
    out.clear();
    out.reserve(m * in.size());
    for (size_t i = 0; i < in.size(); ++i)
    {
        out.insert(out.end(), in[i].begin(), in[i].end());
    }
    return m;
}

} // namespace basecfd

#endif //BASECFD_FORMAT_GUARD_H
