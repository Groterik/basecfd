#ifndef BASECFD_FORMAT_GUARD_H
#define BASECFD_FORMAT_GUARD_H

#include <basecfd/definitions.hpp>
#include <basecfd/grid.hpp>

#include <map>
#include <string>

namespace basecfd
{

class Formater
{
public:
    virtual Grid* load(const std::string& file) = 0;
    virtual void save(const Grid& grid, const std::string& file) = 0;
    virtual ~Formater() {}
};

class FormatFactory
{
public:
    static FormatFactory& instance();
    void registerExtension(const std::string& ext, Formater* formater);
    void unregisterExtension(const std::string& ext);
    void getByExtension(const std::string& ext, Formater* formater);
private:
    FormatFactory();
    typedef std::map<std::string, Formater*> FormatMap;
    FormatMap formats;
};

} // namespace basecfd

#endif //BASECFD_FORMAT_GUARD_H
