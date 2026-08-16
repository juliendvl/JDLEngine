#pragma once

#include "core/object.hpp"

#include "core/events.hpp"
#include "core/render_context.hpp"


namespace jdl
{
namespace scene
{

class SceneNode;

class Scene : public core::Object
{
public:
	/**
	 * @brief Creates a scene.
	 * @param name Scene name.
	 */
	Scene(const std::string& name);

	/**
	 * @brief Returns the root node of the scene graph.
	 */
	const std::shared_ptr<SceneNode>& get_root_node() const {
		return m_rootNode;
	}

	/**
	 * @brief Sets the root node of the scene graph.
	 * @param root_node Root node to assign.
	 */
	void set_root_node(const std::shared_ptr<SceneNode>& root_node) {
		m_rootNode = root_node;
	}

	/**
	 * @brief Renders the scene by traversing the scene graph from the root node.
	 * @param context Render context used for drawing.
	 */
	void render(core::RenderContext& context);

	/**
	 * @brief Resize event handler.
	 * @param event Event data.
	 */
	void resize_event(const core::ResizeEvent& event);

private:
	std::shared_ptr<SceneNode> m_rootNode = nullptr;
};

} // namespace scene
} // namespace jdl
