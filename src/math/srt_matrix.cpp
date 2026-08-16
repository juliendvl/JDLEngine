#include "math/srt_matrix.hpp"

#include <glm/gtx/matrix_decompose.hpp>


namespace jdl
{
namespace math
{

const SRTMatrix SRTMatrix::IDENTITY;

SRTMatrix::SRTMatrix(
	const glm::vec3& p_scale,
	const glm::quat& p_rotation,
	const glm::vec3& p_translation
)
	: scale(p_scale)
	, rotation(p_rotation)
	, translation(p_translation)
{}

SRTMatrix::SRTMatrix(const glm::mat4& matrix)
{
	from_matrix(matrix);
}

void SRTMatrix::get_rotation(glm::vec3& out_axis, float& out_angle) const
{
	out_axis = glm::axis(rotation);
	out_angle = glm::angle(rotation);
}

void SRTMatrix::get_rotation(glm::vec3& out_angles) const
{
	out_angles = glm::eulerAngles(rotation);
}

void SRTMatrix::set_rotation(const glm::vec3& axis, float angle)
{
	rotation = glm::angleAxis(angle, glm::normalize(axis));
}

void SRTMatrix::set_rotation(const glm::vec3& angles)
{
	rotation = glm::quat(angles);
}

glm::mat4 SRTMatrix::to_matrix() const
{
	glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 R = glm::toMat4(rotation);
	glm::mat4 T = glm::translate(glm::mat4(1.0f), translation);

	return T * R * S;
}

void SRTMatrix::from_matrix(const glm::mat4& matrix)
{
	// Unused components
	glm::vec3 _skew;
	glm::vec4 _persp;
	
	glm::decompose(matrix, scale, rotation, translation, _skew, _persp);
}

SRTMatrix SRTMatrix::get_inverse() const
{
	glm::mat4 inverse = glm::inverse(to_matrix());
	return SRTMatrix(inverse);
}

SRTMatrix& SRTMatrix::operator*=(const SRTMatrix& other)
{
	from_matrix(to_matrix() * other.to_matrix());
	return *this;
}

SRTMatrix& SRTMatrix::operator*=(const glm::mat4& other)
{
	from_matrix(to_matrix() * other);
	return *this;
}

SRTMatrix operator*(const SRTMatrix& a, const SRTMatrix& b)
{
	SRTMatrix result(a);
	return result *= b;
}

SRTMatrix operator*(const SRTMatrix& a, const glm::mat4& b)
{
	SRTMatrix result(a);
	return result *= b;
}

glm::mat4 operator*(const glm::mat4& a, const SRTMatrix& b)
{
	return a * b.to_matrix();
}

} // namespace math
} // namespace jdl
