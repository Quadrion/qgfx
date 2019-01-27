#ifndef ishader_h__
#define ishader_h__

#include <qtl/string.h>
#include <qtl/vector.h>

class IShader
{
public:
	IShader() = default;
	virtual ~IShader() = default;

	IShader& operator = (const IShader&) = delete;

	virtual bool attachVertexShader(const qtl::vector<char>& source) = 0;
	virtual bool attachGeometryShader(const qtl::vector<char>& source) = 0;
	virtual bool attachTesselationControlShader(const qtl::vector<char>& source) = 0;
	virtual bool attachTesselationEvaluationShader(const qtl::vector<char>& source) = 0;
	virtual bool attachFragmentShader(const qtl::vector<char>& source) = 0;
	virtual bool compile() = 0;

	virtual bool bind() = 0;
	virtual bool unbind() = 0;
	virtual uint32_t getStageCount() const = 0;
	virtual qtl::vector<void*> getStages() const = 0;
};

#endif // ishader_h__