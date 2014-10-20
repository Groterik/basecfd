#include <basecfd/plugin.hpp>

#include <thirdparty/dynfload/dynfload.hpp>

#include <memory>
#include <stdexcept>
#include <cassert>

using namespace basecfd;
using namespace basecfd::plugin;

typedef Plugin* (*Creator)();
typedef void (*Deleter)(Plugin*);

void PluginManager::Load(const std::string& path)
{
    std::auto_ptr<DynFLoad::DynamicModule> module(new DynFLoad::DynamicModule(path.c_str()));
    Plugins::iterator it = plugins.find(path);
    if (it == plugins.end())
    {
        Creator c = module->getSymbol<Creator>("basecfd_plugin_init");
        Deleter d = module->getSymbol<Deleter>("basecfd_plugin_free");
        if (!c || !d)
        {
            throw std::runtime_error("Failed to load init or release function from plugin " + path);
        }
        Plugin* plugin = c();
        assert(plugin);
        if (!plugin->init())
        {
            throw std::runtime_error("Failed in plugin initialization " + path);
        }
        plugins.insert(std::make_pair(path, std::make_pair(plugin, module.get())));
        module.release();
    }
}

PluginManager& PluginManager::instance()
{
    static PluginManager inst;
    return inst;
}

PluginManager::~PluginManager()
{
    for (Plugins::iterator it = plugins.begin(); it != plugins.end(); ++it)
    {
        Plugin* plugin = it->second.first;
        DynFLoad::DynamicModule* module = it->second.second;
        plugin->release();
        Deleter d = module->getSymbol<Deleter>("basecfd_plugin_free");
        assert(d);
        d(plugin);
        delete module;
    }
}

const char* basecfd::plugin::Plugin::description() const
{
    return "No description provided";
}
