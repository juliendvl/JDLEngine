#pragma once

#include <memory>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>


#define VK_ATTR(vkType, vkAttr) vkType vkAttr = VK_NULL_HANDLE

#define VK_SIZE(vkContainer) static_cast<uint32_t>(vkContainer.size())
#define VK_DATA(vkContainer) vkContainer.data()

#ifndef NDEBUG
    #define VK_CALL(f)  {                                                           \
                            VkResult r = f;                                         \
                            if (r != VK_SUCCESS)                                    \
                                JDL_FATAL("{} failed with status {}", #f, (int)r);  \
                        }

#else
    #define VK_CALL(f) f
#endif
