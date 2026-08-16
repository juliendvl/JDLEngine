#include "scene/scene_node.hpp"

#include "utils/logger.hpp"
#include "utils/string_utils.hpp"


namespace jdl
{
namespace scene
{

const std::string SceneNode::PATH_SEPARATOR = "|";

SceneNode::SceneNode(const std::string& name)
	: core::Object(name)
{}

void SceneNode::add_child(const SceneNodePtr& child)
{
	const auto& child_name = child->get_name();
	
	if (get_child(child_name) != nullptr)
	{
		JDL_ERROR("{} already has a child named {}", get_name(), child_name);
		return;
	}

	auto old_parent = child->get_parent();
	if (old_parent) {
		old_parent->remove_child(child_name);
	}

	m_children[child_name] = child;
	child->m_parent = std::static_pointer_cast<SceneNode>(get_pointer());
	child->set_world_transform_dirty();
}

SceneNodePtr SceneNode::get_child(const std::string& child_name) const
{
	auto it = m_children.find(child_name);
	return it != m_children.end() ? it->second : nullptr;
}

void SceneNode::remove_child(const std::string& child_name)
{
	auto it = m_children.find(child_name);
	if (it == m_children.end()) {
		return;
	}

	it->second->m_parent.reset();
	it->second->set_world_transform_dirty();
	m_children.erase(it);
}

void SceneNode::set_parent(const SceneNodePtr& new_parent)
{
	const auto& node_name = get_name();

	if (new_parent && new_parent->get_child(node_name))
	{
		JDL_ERROR("{} already has a child named {}", new_parent->get_name(), node_name);
		return;
	}

	auto old_parent = get_parent();
	if (old_parent) {
		old_parent->remove_child(node_name);
	}

	if (new_parent)
	{
		new_parent->m_children[node_name] = std::static_pointer_cast<SceneNode>(
			get_pointer()
		);
		m_parent = new_parent;
	}
	set_world_transform_dirty();
}

std::string SceneNode::get_path()
{
	std::vector<std::string> path_nodes;

	SceneNodePtr current = std::static_pointer_cast<SceneNode>(get_pointer());
	while (current)
	{
		path_nodes.push_back(current->get_name());
		current = current->get_parent();
	}

	return StringUtils::Join(path_nodes.rbegin(), path_nodes.rend(), PATH_SEPARATOR);
}

SceneNodePtr SceneNode::find_node(const std::string& node_path)
{
	std::vector<std::string> nodes = StringUtils::Split(node_path, PATH_SEPARATOR);
	if (nodes.empty()) {
		return nullptr;
	}

	SceneNodePtr current = std::static_pointer_cast<SceneNode>(get_pointer());
	for (const std::string& node_name : nodes)
	{
		current = current->get_child(node_name);
		if (!current) {
			return nullptr;
		}
	}

	return current;
}

const math::SRTMatrix& SceneNode::get_world_transform()
{
	if (m_worldTransformDirty)
	{
		auto parent = get_parent();
		m_worldTransform = (parent
			? parent->get_world_transform() * m_parentTransform
			: m_parentTransform
		);

		m_worldTransformDirty = false;
	}

	return m_worldTransform;
}

void SceneNode::set_world_transform(const math::SRTMatrix& transform)
{
	m_worldTransform = transform;

	// Update the parent transform
	auto parent = get_parent();
	m_parentTransform = (
		parent
		? parent->get_world_transform().get_inverse() * m_worldTransform
		: m_worldTransform
	);

	// The world transform must be recomputed for the node descendents
	for (const auto& [name, child] : m_children) {
		child->set_world_transform_dirty();
	}
}

void SceneNode::set_parent_transform(const math::SRTMatrix& transform)
{
	m_parentTransform = transform;
	set_world_transform_dirty();
}

void SceneNode::render(core::RenderContext& context)
{
	if (m_visible)
	{
		do_render(context);
		for (const auto& [name, child] : m_children) {
			child->render(context);
		}
	}
}

void SceneNode::set_world_transform_dirty()
{
	m_worldTransformDirty = true;
	for (const auto& [name, child] : m_children) {
		child->set_world_transform_dirty();
	}
}

} // namespace scene
} // namespace jdl
