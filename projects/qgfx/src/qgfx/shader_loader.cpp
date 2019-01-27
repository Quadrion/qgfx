#include "qgfx/shader_loader.h"

#include "qgfx/qassert.h"

#include <fstream>
#include <iostream>
#include <string>
#include <stddef.h>

qtl::vector<char> load_spirv(const qtl::string file)
{
	std::ifstream is(file.c_str(), std::ios::ate | std::ios::binary);
	QGFX_ASSERT_MSG(is.is_open(), "File %s could not be opened!\n", file.c_str());
	size_t fileSize = static_cast<size_t>(is.tellg());
	qtl::vector<char> buf(fileSize);
	is.seekg(0);
	is.read(buf.data(), fileSize);
	is.close();
	return buf;
}

qtl::vector<char> load_text(const qtl::string file)
{
	std::ifstream is(file.c_str());
	std::string fileData;

	if (is.is_open())
	{
		std::string line;
		while (getline(is, line))
		{
			line += "\n";
			fileData += line.c_str();
		}
		is.close();
	}

	qtl::vector<char> buf;
	buf.reserve(fileData.size());
	
	for (auto c : fileData)
	{
		buf.emplace_back(c);
	}
	buf.emplace_back('\0');

	return buf;
}
