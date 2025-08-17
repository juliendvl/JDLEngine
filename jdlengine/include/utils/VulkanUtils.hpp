#pragma once

#include "core/Core.hpp"


namespace jdl
{
namespace utils
{
namespace VulkanUtils
{

/**
 * @brief  Creates an image view for the source image.
 * @param  image Source image
 * @param  format Image format
 * @param  aspectMask Image aspect mask
 * @return The created image view
 */
JDL_API VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectMask);

} // namespace VulkanUtils
} // namespace utils
} // namespace jdl
