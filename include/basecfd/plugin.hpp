#ifndef BASECFD_PLUGIH_GUARD_H
#define BASECFD_PLUGIH_GUARD_H

#include <basecfd/definitions.hpp>

#include <string>
#include <map>

namespace DynFLoad
{
class DynamicModule;
}

namespace basecfd
{

namespace plugin
{

class Plugin
{
public:
    virtual bool init() = 0;
    virtual void release() = 0;
    virtual const char* description() const;
    virtual ~Plugin() {}
};

class PluginManager
{
public:
    void Load(const std::string& path);
    static PluginManager& instance();
    ~PluginManager();
private:
    typedef std::map<std::string, std::pair<Plugin*, DynFLoad::DynamicModule*> > Plugins;
    Plugins plugins;
};

} // namespace plugin

}

#define BASECFD_REGISTER_PLUGIN(name, pluginType) \
    extern "C" DLL_EXPORT void basecfd_plugin_free(basecfd::plugin::Plugin* p)\
    {\
        delete p;\
    }\
    extern "C" DLL_EXPORT basecfd::plugin::Plugin* basecfd_plugin_init()\
    {\
        return new pluginType;\
    }

#endif // BASECFD_PLUGIH_GUARD_H
