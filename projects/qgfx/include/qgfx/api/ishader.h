#ifndef ishader_h__
#define ishader_h__

#include <string>

class IShader
{
public:
	IShader() = default;
	virtual ~IShader() = 0;

	IShader& operator = (const IShader&) = delete;

	virtual bool attachVertexShader(const std::string& source) = 0;
	virtual bool attachGeometryShader(const std::string& source) = 0;
	virtual bool attachTesselationControlShader(const std::string& source) = 0;
	virtual bool attachTesselationEvaluationShader(const std::string& source) = 0;
	virtual bool attachFragmentShader(const std::string& source) = 0;
	virtual bool compile() = 0;

	virtual bool bind() = 0;
	virtual bool unbind() = 0;
};

#endif // ishader_h__