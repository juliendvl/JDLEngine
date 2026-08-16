#include "utils/string_utils.hpp"


namespace jdl
{
namespace StringUtils
{

std::string Join(const std::vector<std::string>& seq, const std::string& sep) {
	return Join(seq.cbegin(), seq.cend(), sep);
}

std::vector<std::string> Split(const std::string& str, const std::string& sep)
{
	std::vector<std::string> result;

	auto start = 0U;
	auto end = str.find(sep, start);

	while (end != std::string::npos)
	{
		result.push_back(str.substr(start, end - start));

		start = end + sep.length();
		end = str.find(sep, start);
	}

	// Add the last substring to the result
	result.push_back(str.substr(start, end));
	return result;
}

} // namespace StringUtils
} // namespace jdl
