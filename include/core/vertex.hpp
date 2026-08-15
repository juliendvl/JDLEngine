#pragma once


namespace jdl
{
namespace core
{

struct Vertex
{
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 normal = glm::vec3(0.0f);
	glm::vec2 uv = glm::vec2(0.0f);

	Vertex() = default;

	Vertex(
		const glm::vec3& p_position,
		const glm::vec3& p_normal,
		const glm::vec2& p_uv
	)
		: position(p_position)
		, normal(p_normal)
		, uv(p_uv)
	{}
};

} // namespace core
} // namespace jdl
