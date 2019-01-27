#ifndef shaderloader_h__
#define shaderloader_h__

#include <qtl/string.h>
#include <qtl/vector.h>

qtl::vector<char> load_spirv(const qtl::string file);
qtl::vector<char> load_text(const qtl::string file);

#endif // shaderloader_h__