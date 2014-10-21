#include <basecfd/format.hpp>

#include <stdexcept>

using namespace basecfd;

FormatFactory::FormatFactory()
{

}

FormatFactory& FormatFactory::instance()
{
    static FormatFactory inst;
    return inst;
}

void FormatFactory::registerExtension(const std::string& ext, Formater* formater)
{
    FormatMap::iterator it = formats.find(ext);
    if (it == formats.end())
    {
        formats.insert(std::make_pair(ext, formater));
    }
    throw std::runtime_error("Failed to register format with extension " +
                             ext + " (already used)");
}

void FormatFactory::unregisterExtension(const std::string& ext)
{
    FormatMap::iterator it = formats.find(ext);
    if (it != formats.end())
    {
        formats.erase(it);
    }
    throw std::runtime_error("Failed to unregister format with extension " +
                             ext + " (not registered)");
}

Formater* FormatFactory::getByExtension(const std::string& ext)
{
    FormatMap::iterator it = formats.find(ext);
    return it != formats.end() ? it->second : throw std::runtime_error("Failed to detect extension " +
                                                                       ext + "(not registered)");
}
