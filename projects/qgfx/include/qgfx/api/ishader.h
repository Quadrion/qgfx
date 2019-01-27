#ifndef ishader_h__
#define ishader_h__

#include <qtl/string.h>

class IShader
{
public:
	IShader() = default;
	virtual ~IShader() = 0;

	IShader& operator = (const IShader&) = delete;

	virtual bool attachVertexShader(const qtl::string& source) = 0;
	virtual bool attachGeometryShader(const qtl::string& source) = 0;
	virtual bool attachTesselationControlShader(const qtl::string& source) = 0;
	virtual bool attachTesselationEvaluationShader(const qtl::string& source) = 0;
	virtual bool attachFragmentShader(const qtl::string& source) = 0;
	virtual bool compile() = 0;

	virtual bool bind() = 0;
	virtual bool unbind() = 0;
};

#endif // ishader_h__