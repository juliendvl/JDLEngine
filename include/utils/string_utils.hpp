#pragma once


namespace jdl
{
namespace StringUtils
{

/**
 * @brief Joins the elements in an iterator range into a single string.
 * @param start Start iterator of the range.
 * @param end End iterator of the range.
 * @param sep Separator inserted between consecutive elements.
 * @return The joined string.
 */
template<typename Iter>
std::string Join(Iter start, Iter end, const std::string& sep)
{
	std::string result = *start;

	for (auto it = std::next(start); it != end; ++it) {
		result.append(sep + *it);
	}

	return result;
}

/**
 * @brief Joins the strings in a sequence into a single string.
 * @param seq Sequence of strings to join.
 * @param sep Separator inserted between consecutive elements.
 * @return The joined string.
 */
std::string Join(const std::vector<std::string>& seq, const std::string& sep);

/**
 * @brief Splits a string into substrings using the given separator.
 * @param str String to split.
 * @param sep Separator used to divide the string.
 * @return A vector containing the split substrings.
 */
std::vector<std::string> Split(const std::string& str, const std::string& sep);

} // namespace StringUtils
} // namespace jdl
