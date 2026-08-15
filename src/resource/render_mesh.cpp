#include "resource/render_mesh.hpp"


namespace jdl
{
namespace resource
{

RenderMesh::RenderMesh(const std::string& name)
	: Resource(name)
{}

void RenderMesh::add_vertex(const core::Vertex& vertex)
{
	m_vertices.push_back(vertex);
	m_reloadVertices = true;
}

void RenderMesh::add_index(uint32_t index)
{
	m_indices.push_back(index);
	m_reloadIndices = true;
}

void RenderMesh::render(core::RenderContext& context)
{
	update_buffers();

	if (m_vertexBuffer == nullptr) {
		return;
	}
	m_vertexBuffer->bind(*context.command_buffer);

	if (m_indices.empty())
	{
		uint32_t nb_vertices = static_cast<uint32_t>(m_vertices.size());
		context.command_buffer->draw(nb_vertices);
	}
	else
	{
		m_indexBuffer->bind(*context.command_buffer);

		uint32_t nb_indices = static_cast<uint32_t>(m_indices.size());
		context.command_buffer->draw_indexed(nb_indices);
	}
}

void RenderMesh::update_buffers()
{
	if (m_reloadVertices)
	{
		m_vertexBuffer.reset();
		m_vertexBuffer = std::make_unique<vk::VertexBuffer>(
			m_vertices.size() * sizeof(core::Vertex)
		);
		m_vertexBuffer->set_data(m_vertices.data());

		m_reloadVertices = false;
	}
	if (m_reloadIndices)
	{
		m_indexBuffer.reset();
		m_indexBuffer = std::make_unique<vk::IndexBuffer>(
			m_indices.size() * sizeof(uint32_t)
		);
		m_indexBuffer->set_data(m_indices.data());

		m_reloadIndices = false;
	}
}

void RenderMesh::clear_resource()
{
	m_indexBuffer.reset();
	m_vertexBuffer.reset();
}

} // namespace resource
} // namespace jdl
