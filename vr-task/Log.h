#pragma once

#include <iostream>
#include <fstream>
#include <time.h>

#include "Math.h"
#include "Time.h"

namespace std
{
	template<glm::length_t L, class T>
	ostream& operator<<(ostream& stream, const glm::vec<L, T>& vec) {
		stream << "[ ";
		for (size_t i = 0; i < L; ++i) {
			stream << vec[i];

			if (i + 1 != L) {
				stream << ", ";
			}
		}
		stream << " ]";

		return stream;
	}
}

class Log
{
public:
	// Creates output file
	static void init(const std::string& path = "log.txt");

	// Prints specified parameters to console and file 
	// Adds spaces between them and adds new line symbol at the end
	template<class Arg, class... Args>
	static void write(Arg&& arg, Args&&... args)
	{
		writeToStream(m_file, arg, args...);
		writeToStream(std::cout, arg, args...);
	}
private:
	// One magic function to 
	template<class Arg, class... Args>
	static void writeToStream(std::ostream& out, Arg&& arg, Args&&... args)
	{
		out << "[" << getDateTime() << "] " << std::forward<Arg>(arg);
		using dummy = int[];
		(void)dummy {
			0, (void(out << ' ' << std::forward<Args>(args)), 0)...
		};

		out << std::endl;
	}

	static std::ofstream m_file;
};