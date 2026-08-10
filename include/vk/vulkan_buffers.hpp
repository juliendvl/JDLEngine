#pragma once

#include "vulkan_command_buffer.hpp"
#include "vulkan_utils.hpp"

#include "utils/non_copyable.hpp"


namespace jdl
{
namespace vk
{

enum BufferUsage
{
	eTransferSrc = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	eTransferDst = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
	eIndex = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
	eVertex = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
};

inline constexpr BufferUsage operator|(BufferUsage a, BufferUsage b) {
	return static_cast<BufferUsage>(static_cast<int>(a) | static_cast<int>(b));
}

inline constexpr BufferUsage operator&(BufferUsage a, BufferUsage b) {
	return static_cast<BufferUsage>(static_cast<int>(a) & static_cast<int>(b));
}

inline constexpr BufferUsage operator^(BufferUsage a, BufferUsage b) {
	return static_cast<BufferUsage>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline constexpr BufferUsage operator~(BufferUsage a) {
	return static_cast<BufferUsage>(~static_cast<int>(a));
}

inline BufferUsage& operator|=(BufferUsage& a, BufferUsage b) {
	a = a | b;
	return a;
}

inline BufferUsage& operator&=(BufferUsage& a, BufferUsage b) {
	a = a & b;
	return a;
}

inline BufferUsage& operator^=(BufferUsage& a, BufferUsage b) {
	a = a ^ b;
	return a;
}


class VulkanBuffer : private NonCopyable<VulkanBuffer>
{
public:
	/**
	 * @brief Creates a Vulkan buffer and allocates its device memory.
	 * @param size The size of the buffer in bytes.
	 * @param usage Buffer usage flags.
	 * @param memory_properties Memory properties required for the allocation.
	 */
	VulkanBuffer(uint64_t size, BufferUsage usage, MemoryProperty memory_properties);

	/**
	 * @brief Destroys the buffer and frees its device memory.
	 */
	~VulkanBuffer();

	/**
	 * @brief Returns the buffer size in bytes.
	 */
	uint64_t get_size() const { return m_size; }

	/**
	 * @brief Returns the Vulkan buffer handle.
	 */
	VkBuffer get() const { return m_buffer; }

	/**
	 * @brief Copies data into the buffer's mapped memory.
	 * @param data Pointer to the source data.
	 * @param size Number of bytes to copy. Defaults to the full buffer size.
	 * @param offset Byte offset within the buffer where the copy starts.
	 */
	void set_data(void* data, uint64_t size = 0, uint64_t offset = 0);

private:
	VK_ATTR(VkDevice, m_device);
	VK_ATTR(VkBuffer, m_buffer);
	VK_ATTR(VkDeviceMemory, m_deviceMemory);

	uint64_t m_size = 0;

	void create_buffer(BufferUsage usage);
	void allocate_memory(MemoryProperty memory_properties);
};


class Buffer : private NonCopyable<Buffer>
{
public:
	/**
	 * @brief Creates a host-visible staging buffer and a device-local buffer.
	 * @param size The size of both buffers in bytes.
	 * @param usage Device buffer usage flags.
	 */
	Buffer(uint64_t size, BufferUsage usage);

	/**
	 * @brief Returns the buffer size in bytes.
	 */
	uint64_t get_size() const { return m_stagingBuffer.get_size(); }

	/**
	 * @brief Returns the host-visible staging buffer object.
	 */
	const VulkanBuffer& get_staging_buffer() const { return m_stagingBuffer; }

	/**
	 * @brief Returns the device-local buffer object.
	 */
	const VulkanBuffer& get_device_buffer() const { return m_deviceBuffer; }

	/**
	 * @brief Uploads data to the staging buffer and copies it to the device buffer.
	 * @param data Pointer to the source data.
	 * @param size Number of bytes to copy. Defaults to the full buffer size.
	 * @param offset Byte offset within the buffer where the copy starts.
	 */
	void set_data(void* data, uint64_t size = 0, uint64_t offset = 0);

private:
	VulkanBuffer m_stagingBuffer;
	VulkanBuffer m_deviceBuffer;
};


class VertexBuffer : public Buffer
{
public:
	/**
	 * @brief Creates a vertex buffer with the given size.
	 * @param size The size of the buffer in bytes.
	 */
	VertexBuffer(uint64_t size);

	/**
	 * @brief Records the command to bind this vertex buffer.
	 * @param command_buffer Command buffer used to record the bind command.
	 */
	void bind(const VulkanCommandBuffer& command_buffer) const;
};


class IndexBuffer : public Buffer
{
public:
	/**
	 * @brief Creates an index buffer with the given size.
	 * @param size The size of the buffer in bytes.
	 */
	IndexBuffer(uint64_t size);

	/**
	 * @brief Records the command to bind this index buffer.
	 * @param command_buffer Command buffer used to record the bind command.
	 */
	void bind(const VulkanCommandBuffer& command_buffer) const;
};

} // namespace vk
} // namespace jdl
