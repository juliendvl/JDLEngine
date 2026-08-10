#include "vk/vulkan_utils.hpp"
#include "vk/vulkan_context.hpp"

#include "utils/logger.hpp"


namespace jdl
{
namespace vk
{

namespace VulkanUtils
{

VkDeviceMemory CreateDeviceMemory(
	VkDevice device,
	VkPhysicalDevice physical_device,
	VkMemoryRequirements memory_requirements,
	VkMemoryPropertyFlags memory_properties
)
{
	// Find a suitable memory type
	VkPhysicalDeviceMemoryProperties device_memories;
	vkGetPhysicalDeviceMemoryProperties(physical_device, &device_memories);

	uint32_t memory_type = UINT32_MAX;

	for (uint32_t i = 0; i < device_memories.memoryTypeCount; ++i)
	{
		// Check compatibility with memory requirements
		uint32_t compatible = memory_requirements.memoryTypeBits & (1 << i);
		if (compatible == 0) {
			continue;
		}

		// Check compatibility with memory properties
		compatible = device_memories.memoryTypes[i].propertyFlags & memory_properties;
		if (compatible != memory_properties) {
			continue;
		}

		// The memory type is compatible
		memory_type = i;
		break;
	}
	if (memory_type == UINT32_MAX)
	{
		JDL_ERROR("Cannot find a suitable memory type");
		return VK_NULL_HANDLE;
	}

	VkMemoryAllocateInfo alloc_info {
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memory_requirements.size,
		.memoryTypeIndex = memory_type
	};

	VkDeviceMemory device_memory;
	VK_CALL(vkAllocateMemory(device, &alloc_info, nullptr, &device_memory));

	return device_memory;
}

} // namespace VulkanUtils

} // namespace vk
} // namespace jdl
