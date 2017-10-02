#include "Stuff.h"

std::vector<std::string> split(const std::string & string, char delimer)
{
	std::vector<std::string> results;

	auto start = string.begin();
	auto end = string.end();
	auto next = std::find(start, end, delimer);

	while (next != end) {
		results.push_back(std::string(start, next));
		start = next + 1;
		next = std::find(start, end, delimer);
	}
	results.push_back(std::string(start, next));

	return results;
}
