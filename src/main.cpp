#include <iostream>

#include "core/application.hpp"

#include "math/srt_matrix.hpp"

#include "resource/render_mesh.hpp"
#include "resource/resource_manager.hpp"

#include "scene/mesh_node.hpp"
#include "scene/scene.hpp"

#include "utils/logger.hpp"

using namespace jdl;


class Sandbox : public core::Application
{
public:
    Sandbox(const char* name, int width, int height)
        : core::Application(name, width, height)
    {
        auto mesh = resource::ResourceManager::Create<resource::RenderMesh>("MESH");
        mesh->add_vertices({
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}
        });
        mesh->add_indices({ 0, 1, 2, 2, 3, 0 });

        math::SRTMatrix transform;
        transform.scale = glm::vec3(0.5f);
        transform.set_rotation(glm::vec3(0.0f, 0.0f, glm::radians(45.0f)));
        transform.translation = glm::vec3(0.2f, -0.3f, 0.0f);

        auto node = scene::CreateNode<scene::MeshNode>("NODE", nullptr, mesh);
        node->set_world_transform(transform);

        auto scene = std::make_shared<scene::Scene>("SCENE");
        scene->set_root_node(node);

        GetRenderer().set_scene(scene);
    }
};


int main(int argc, char** argv)
{
    try
    {
        utils::Logger::Init();

        Sandbox application("JDLEngine", 800, 600);
        application.run();

        return EXIT_SUCCESS;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
