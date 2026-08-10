#pragma once


namespace jdl
{
namespace vk
{

enum MemoryProperty
{
	eDeviceLocal = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
	eHostVisible = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
	eHostCoherent = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
};

inline constexpr MemoryProperty operator|(MemoryProperty a, MemoryProperty b) {
	return static_cast<MemoryProperty>(static_cast<int>(a) | static_cast<int>(b));
}

inline constexpr MemoryProperty operator&(MemoryProperty a, MemoryProperty b) {
	return static_cast<MemoryProperty>(static_cast<int>(a) & static_cast<int>(b));
}

inline constexpr MemoryProperty operator^(MemoryProperty a, MemoryProperty b) {
	return static_cast<MemoryProperty>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline constexpr MemoryProperty operator~(MemoryProperty a) {
	return static_cast<MemoryProperty>(~static_cast<int>(a));
}

inline MemoryProperty& operator|=(MemoryProperty& a, MemoryProperty b) {
	a = a | b;
	return a;
}

inline MemoryProperty& operator&=(MemoryProperty& a, MemoryProperty b) {
	a = a & b;
	return a;
}

inline MemoryProperty& operator^=(MemoryProperty& a, MemoryProperty b) {
	a = a ^ b;
	return a;
}


namespace VulkanUtils
{

/**
 * @brief Allocates device memory matching the given requirements and properties.
 * @param device Logical device used for the allocation.
 * @param physical_device Physical device used to query memory types.
 * @param memory_requirements Memory requirements of the resource to allocate for.
 * @param memory_properties Required memory property flags.
 * @return The allocated device memory handle, or VK_NULL_HANDLE on failure.
 */
VkDeviceMemory CreateDeviceMemory(
	VkDevice device,
	VkPhysicalDevice physical_device,
	VkMemoryRequirements memory_requirements,
	VkMemoryPropertyFlags memory_properties
);

} // namespace VulkanUtils

} // namespace vk
} // namespace jdl
