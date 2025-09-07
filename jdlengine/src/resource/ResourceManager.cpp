#include "resource/ResourceManager.hpp"
#include "resource/Resource.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace resource
{

std::unordered_map<std::string, std::unique_ptr<Resource>> ResourceManager::RESOURCES;

Resource* ResourceManager::Get(const std::string& name)
{
    auto it = RESOURCES.find(name);
    return it != RESOURCES.end() ? it->second.get() : nullptr;
}

void ResourceManager::Remove(const std::string& name)
{
    auto it = RESOURCES.find(name);
    if (it != RESOURCES.end())
    {
        it->second->clearResourceData();
        RESOURCES.erase(it);
    }
}

void ResourceManager::Clear()
{
    for (const auto& it : RESOURCES) {
        it.second->clearResourceData();
    }
    RESOURCES.clear();
}

} // namespace resource
} // namespace jdl
