#pragma once

#include "core/object.hpp"
#include "core/render_context.hpp"

#include "math/srt_matrix.hpp"

#include <unordered_map>


namespace jdl
{
namespace scene
{

class SceneNode : public core::Object
{
public:
	/**
	 * @brief Creates a scene node.
	 * @param name Node name.
	 */
	SceneNode(const std::string& name);

	/**
	 * @brief Adds a child node to this node.
	 *
	 * If the child already has a parent, it is removed from the previous parent
	 * first. Duplicate child names under the same parent are rejected.
	 * @param child Child node to add.
	 */
	void add_child(const std::shared_ptr<SceneNode>& child);

	/**
	 * @brief Returns a direct child node by name.
	 * @param child_name Name of the child to look up.
	 * @return Shared pointer to the child, or nullptr if not found.
	 */
	std::shared_ptr<SceneNode> get_child(const std::string& child_name) const;

	/**
	 * @brief Removes a direct child node by name.
	 * @param child_name Name of the child to remove.
	 */
	void remove_child(const std::string& child_name);

	/**
	 * @brief Returns the parent node.
	 * @return Shared pointer to the parent, or nullptr if this is a root node.
	 */
	std::shared_ptr<SceneNode> get_parent() const { return m_parent.lock(); }

	/**
	 * @brief Reparents this node under a new parent.
	 *
	 * If this node already has a parent, it is removed from the previous parent
	 * first. Duplicate child names under the new parent are rejected.
	 * @param new_parent New parent node, or nullptr to detach from any parent.
	 */
	void set_parent(const std::shared_ptr<SceneNode>& new_parent);

	/**
	 * @brief Returns the path of this node from the root.
	 *
	 * Node names are joined with PATH_SEPARATOR, starting from the root.
	 * @return Path string for this node.
	 */
	std::string get_path();

	/**
	 * @brief Finds a descendant node by path.
	 *
	 * The search starts at this node. Path segments are separated by
	 * PATH_SEPARATOR.
	 * @param node_path Path to the target node.
	 * @return Shared pointer to the found node, or nullptr if not found.
	 */
	std::shared_ptr<SceneNode> find_node(const std::string& node_path);

	/**
	 * @brief Returns the world-space transform of this node.
	 */
	const math::SRTMatrix& get_world_transform();

	/**
	 * @brief Returns the transform of this node relative to its parent.
	 */
	const math::SRTMatrix& get_parent_transform() const { return m_parentTransform; }

	/**
	 * @brief Sets the world-space transform of this node.
	 * @param transform World-space transform.
	 */
	void set_world_transform(const math::SRTMatrix& transform);
	void set_world_transform(const glm::mat4& transform) {
		set_world_transform(math::SRTMatrix(transform));
	}

	/**
	 * @brief Sets the transform of this node relative to its parent.
	 * @param transform Parent-relative transform.
	 */
	void set_parent_transform(const math::SRTMatrix& transform);
	void set_parent_transform(const glm::mat4& transform) {
		set_parent_transform(math::SRTMatrix(transform));
	}

	/**
	 * @brief Returns whether this node is visible.
	 */
	bool is_visible() const { return m_visible; }

	/**
	 * @brief Sets whether this node is visible.
	 *
	 * Invisible nodes skip rendering for themselves and their descendants.
	 * @param visible Visibility flag.
	 */
	void set_visible(bool visible) { m_visible = visible; }

	/**
	 * @brief Renders this node and its visible descendants.
	 * @param context Render context used for drawing.
	 */
	void render(core::RenderContext& context);

	/** @brief Separator used in node paths returned by get_path() and find_node(). */
	static const std::string PATH_SEPARATOR;

protected:
	/**
	 * @brief Performs node-specific rendering.
	 *
	 * Called by render() when the node is visible. Override in derived classes
	 * to draw node content.
	 * @param context Render context used for drawing.
	 */
	virtual void do_render(core::RenderContext& context) {}

private:
	std::unordered_map<std::string, std::shared_ptr<SceneNode>> m_children;
	std::weak_ptr<SceneNode> m_parent;

	math::SRTMatrix m_worldTransform;
	math::SRTMatrix m_parentTransform;

	bool m_worldTransformDirty = false;

	bool m_visible = true;

	void set_world_transform_dirty();
};

/** @brief Shared pointer alias for SceneNode. */
using SceneNodePtr = std::shared_ptr<SceneNode>;


/**
 * @brief Creates and returns a scene node.
 * @param name Node name.
 * @param parent Node parent.
 * @param args Node initialization arguments.
 * @return The created node.
 */
template<class N, typename... Args>
std::shared_ptr<N> CreateNode(
	const std::string& name,
	const std::shared_ptr<SceneNode>& parent,
	Args&&... args
) {
	std::string valid_name = name;
	if (parent)
	{
		size_t index = 1;
		while (parent->get_child(valid_name)) {
			valid_name = name + std::to_string(index++);
		}
	}

	auto node = std::make_shared<N>(name, std::forward<Args>(args)...);
	if (parent) {
		parent->add_child(node);
	}

	return node;
}

} // namespace scene
} // namespace jdl
