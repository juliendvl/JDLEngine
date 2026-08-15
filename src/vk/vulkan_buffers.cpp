#include "vk/vulkan_buffers.hpp"
#include "vk/vulkan_context.hpp"

#include "utils/logger.hpp"


namespace jdl
{
namespace vk
{

// --- VulkanBuffer CLASS ---

VulkanBuffer::VulkanBuffer(
	uint64_t size,
	BufferUsage usage,
	MemoryProperty memory_properties
)
{
	m_device = VulkanContext::GetDevice().get_device();
	m_size = size;

	create_buffer(usage);
	allocate_memory(memory_properties);
}

VulkanBuffer::~VulkanBuffer()
{
	vkFreeMemory(m_device, m_deviceMemory, nullptr);
	vkDestroyBuffer(m_device, m_buffer, nullptr);
}

void VulkanBuffer::set_data(void* data, uint64_t size, uint64_t offset)
{
	if (size == 0) {
		size = m_size;
	}
	if (offset + size > m_size)
	{
		JDL_ERROR("Cannot set Vulkan buffer data: write range exceeds buffer size");
		return;
	}

	void* ptr;
	VK_CALL(vkMapMemory(m_device, m_deviceMemory, offset, size, 0, &ptr));
	{
		memcpy(ptr, data, size);
	}
	vkUnmapMemory(m_device, m_deviceMemory);
}

void VulkanBuffer::create_buffer(BufferUsage usage)
{
	VkBufferCreateInfo create_info {
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = m_size,
		.usage = static_cast<VkBufferUsageFlags>(usage),
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE
	};
	VK_CALL(vkCreateBuffer(m_device, &create_info, nullptr, &m_buffer));
}

void VulkanBuffer::allocate_memory(MemoryProperty memory_properties)
{
	VkMemoryRequirements memory_requirements;
	vkGetBufferMemoryRequirements(m_device, m_buffer, &memory_requirements);

	m_deviceMemory = VulkanUtils::CreateDeviceMemory(
		m_device,
		VulkanContext::GetDevice().get_physical_device(),
		memory_requirements,
		static_cast<VkMemoryPropertyFlags>(memory_properties)
	);
	if (m_deviceMemory == VK_NULL_HANDLE) {
		JDL_FATAL("Failed to allocate memory for the Vulkan buffer");
	}

	VK_CALL(vkBindBufferMemory(m_device, m_buffer, m_deviceMemory, 0));
}


// --- Buffer CLASS ---

Buffer::Buffer(uint64_t size, BufferUsage usage) :
	m_stagingBuffer(
		size,
		BufferUsage::eTransferSrc,
		MemoryProperty::eHostVisible | MemoryProperty::eHostCoherent
	),
	m_deviceBuffer(
		size,
		BufferUsage::eTransferDst | usage,
		MemoryProperty::eDeviceLocal
	)
{}

void Buffer::set_data(void* data, uint64_t size, uint64_t offset)
{
	if (size == 0) {
		size = get_size();
	}
	if (offset + size > get_size())
	{
		JDL_ERROR("Cannot set buffer data: write range exceeds buffer size");
		return;
	}

	// Update the staging buffer
	m_stagingBuffer.set_data(data, size, offset);

	// Copy the content to the device buffer
	VkCommandPool command_pool = VulkanContext::GetDevice().get_graphics_command_pool();
	VulkanCommandBuffer command_buffer(command_pool);

	command_buffer.begin();
	{
		VkBufferCopy copy_region {
			.srcOffset = offset,
			.dstOffset = offset,
			.size = size
		};
		vkCmdCopyBuffer(
			command_buffer.get(),
			m_stagingBuffer.get(),
			m_deviceBuffer.get(),
			1, &copy_region
		);
	}
	command_buffer.end();

	VkQueue queue = VulkanContext::GetDevice().get_graphics_queue();
	command_buffer.submit(queue);
	VK_CALL(vkQueueWaitIdle(queue));

	command_buffer.destroy();
}


// --- VertexBuffer CLASS ---

VertexBuffer::VertexBuffer(uint64_t size)
	: Buffer(size, BufferUsage::eVertex)
{}

void VertexBuffer::bind(const VulkanCommandBuffer& command_buffer) const
{
	VkBuffer buffers[] = { get_device_buffer().get() };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(command_buffer.get(), 0, 1, buffers, offsets);
}


// --- VertexBuffer CLASS ---

IndexBuffer::IndexBuffer(uint64_t size)
	: Buffer(size, BufferUsage::eIndex)
{}

void IndexBuffer::bind(const VulkanCommandBuffer& command_buffer) const
{
	vkCmdBindIndexBuffer(
		command_buffer.get(),
		get_device_buffer().get(),
		0,
		VK_INDEX_TYPE_UINT32
	);
}


} // namespace vk
} // namespace jdl
