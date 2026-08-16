#include "scene/scene.hpp"
#include "scene/scene_node.hpp"


namespace jdl
{
namespace scene
{

Scene::Scene(const std::string& name)
	: core::Object(name)
{}

void Scene::render(core::RenderContext& context)
{
	if (m_rootNode) {
		m_rootNode->render(context);
	}
}

void Scene::resize_event(const core::ResizeEvent& event) {}

} // namespace scene
} // namespace jdl
