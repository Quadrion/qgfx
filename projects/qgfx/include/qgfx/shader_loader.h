#ifndef shaderloader_h__
#define shaderloader_h__

#include <qtl/string.h>
#include <qtl/vector.h>

qtl::vector<char> loadSpirv(const qtl::string& file);
qtl::vector<char> loadText(const qtl::string& file);

#endif // shaderloader_h__