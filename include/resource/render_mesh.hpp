#pragma once

#include "resource.hpp"

#include "core/render_context.hpp"
#include "core/vertex.hpp"

#include "vk/vulkan_buffers.hpp"


namespace jdl
{
namespace resource
{

class RenderMesh : public Resource
{
public:
	/**
	 * @brief Creates a render mesh resource.
	 * @param name Render mesh name.
	 */
	RenderMesh(const std::string& name);

	/**
	 * @brief Appends a single vertex to the mesh.
	 * @param vertex Vertex to add.
	 */
	void add_vertex(const core::Vertex& vertex);

	/**
	 * @brief Appends a range of vertices to the mesh.
	 * @param start Iterator to the first vertex.
	 * @param end Iterator past the last vertex.
	 */
	template<typename Iter>
	void add_vertices(Iter start, Iter end)
	{
		m_vertices.insert(m_vertices.end(), start, end);
		m_reloadVertices = true;
	}

	/**
	 * @brief Appends vertices from a vector to the mesh.
	 * @param vertices Vertices to add.
	 */
	void add_vertices(const std::vector<core::Vertex>& vertices) {
		add_vertices(vertices.cbegin(), vertices.cend());
	}

	/**
	 * @brief Appends a single index to the mesh.
	 * @param index Index to add.
	 */
	void add_index(uint32_t index);

	/**
	 * @brief Appends a range of indices to the mesh.
	 * @param start Iterator to the first index.
	 * @param end Iterator past the last index.
	 */
	template<typename Iter>
	void add_indices(Iter start, Iter end)
	{
		m_indices.insert(m_indices.end(), start, end);
		m_reloadIndices = true;
	}

	/**
	 * @brief Appends indices from a vector to the mesh.
	 * @param indices Indices to add.
	 */
	void add_indices(const std::vector<uint32_t>& indices) {
		add_indices(indices.cbegin(), indices.cend());
	}

	/**
	 * @brief Returns the number of vertices in the mesh.
	 */
	size_t get_nb_vertices() const { return m_vertices.size(); }

	/**
	 * @brief Returns the number of indices in the mesh.
	 */
	size_t get_nb_indices() const { return m_indices.size(); }

	/**
	 * @brief Records draw commands for this mesh into the given render context.
	 * @param context Render context providing the command buffer to record into.
	 */
	void render(core::RenderContext& context);

private:
	std::vector<core::Vertex> m_vertices;
	std::vector<uint32_t> m_indices;

	bool m_reloadVertices = false;
	bool m_reloadIndices = false;

	std::unique_ptr<vk::VertexBuffer> m_vertexBuffer;
	std::unique_ptr<vk::IndexBuffer> m_indexBuffer;

	void update_buffers();
	void clear_resource();
};

} // namespace resource
} // namespace jdl
