#include "scene/mesh_node.hpp"


namespace jdl
{
namespace scene
{

MeshNode::MeshNode(const std::string& name)
	: SceneNode(name)
{}

MeshNode::MeshNode(const std::string& name, resource::RenderMesh* mesh)
	: SceneNode(name)
	, m_mesh(mesh)
{}

void MeshNode::do_render(core::RenderContext& context)
{
	if (m_mesh)
	{
		glm::mat4 model = get_world_transform().to_matrix();
		context.command_buffer->update_model_matrix(model);

		m_mesh->render(context);
	}
}

} // namespace scene
} // namespace jdl
