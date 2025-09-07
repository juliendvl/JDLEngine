#pragma once

#include "core/Object.hpp"


namespace jdl
{
namespace resource
{

class JDL_API Resource : public core::Object
{
    friend class ResourceManager;

public:
    ~Resource() { clearResourceData(); }

protected:
    // Base constructor
    Resource(const std::string& name) : core::Object(name) {}

    /**
     * @brief Clears the resource data. Must be reimplemented if necessary.
     */
    virtual void clearResourceData() {}
};

} // namespace resource
} // namespace jdl
