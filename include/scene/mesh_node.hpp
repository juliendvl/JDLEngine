#pragma once

#include "scene_node.hpp"

#include "resource/render_mesh.hpp"


namespace jdl
{
namespace scene
{

class MeshNode : public SceneNode
{
public:
	/**
	 * @brief Creates a mesh node.
	 * @param name Node name.
	 */
	MeshNode(const std::string& name);

	/**
	 * @brief Creates a mesh node, with a render mesh.
	 * @param name Node name.
	 * @param mesh The initial render mesh.
	 */
	MeshNode(const std::string& name, resource::RenderMesh* mesh);

	/**
	 * @brief Returns the node rendered mesh.
	 */
	resource::RenderMesh* get_mesh() const { return m_mesh; }

	/**
	 * @brief Sets the node rendered mesh.
	 * @param mesh The new render mesh.
	 */
	void set_mesh(resource::RenderMesh* mesh) { m_mesh = mesh; }

private:
	resource::RenderMesh* m_mesh = nullptr;

	void do_render(core::RenderContext& context);
};

} // namespace scene
} // namespace jdl
