#pragma once

#include <glm/gtx/quaternion.hpp>


namespace jdl
{
namespace math
{

struct SRTMatrix
{
	glm::vec3 scale = glm::vec3(1.0f);
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 translation = glm::vec3(0.0f);

	/**
	 * @brief Creates an identity SRT matrix.
	 */
	SRTMatrix() = default;

	/**
	 * @brief Creates a SRT matrix from components.
	 * @param p_scale Scale component.
	 * @param p_rotation Rotation component (quaternion).
	 * @param p_translation Translation component.
	 */
	SRTMatrix(
		const glm::vec3& p_scale,
		const glm::quat& p_rotation,
		const glm::vec3& p_translation
	);

	/**
	 * @brief Creates a SRT matrix from a transform matrix.
	 * @param matrix Source 4x4 transform matrix.
	 */
	SRTMatrix(const glm::mat4& matrix);

	/**
	 * @brief Returns the rotation component (axis-angle representation).
	 * @param out_axis Output normalized rotation axis.
	 * @param out_angle Output rotation angle, in radians.
	 */
	void get_rotation(glm::vec3& out_axis, float& out_angle) const;

	/**
	 * @brief Returns the rotation component (Euler angles representation).
	 * @param out_angles Output angles, in radians.
	 */
	void get_rotation(glm::vec3& out_angles) const;

	/**
	 * @brief Sets the rotation component (axis-angle representation).
	 * @param axis Rotation axis.
	 * @param angle Rotation angle in radians.
	 */
	void set_rotation(const glm::vec3& axis, float angle);

	/**
	 * @brief Sets the rotation component (Euler angles).
	 * @param angles Euler angles, in radians.
	 */
	void set_rotation(const glm::vec3& angles);

	/**
	 * @brief Converts the SRT components to a 4x4 transform matrix.
	 */
	glm::mat4 to_matrix() const;

	/**
	 * @brief Sets the SRT components from a transform matrix.
	 * @param matrix Source 4x4 transform matrix.
	 */
	void from_matrix(const glm::mat4& matrix);

	/**
	 * @brief Computes and returns the inverse SRT matrix.
	 */
	SRTMatrix get_inverse() const;

	SRTMatrix& operator*=(const SRTMatrix& other);
	SRTMatrix& operator*=(const glm::mat4& other);

	static const SRTMatrix IDENTITY;
};

SRTMatrix operator*(const SRTMatrix& a, const SRTMatrix& b);
SRTMatrix operator*(const SRTMatrix& a, const glm::mat4& b);
glm::mat4 operator*(const glm::mat4& a, const SRTMatrix& b);

} // namespace math
} // namespace jdl
