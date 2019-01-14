#ifndef ishader_h__
#define ishader_h__

class IShader
{
public:
	IShader() = default;
	virtual ~IShader() = 0;

	IShader& operator=(const IShader&) = delete;

	virtual bool attachVertexShader(const char * source) = 0;
	virtual bool attachGeometryShader(const char * source) = 0;
	virtual bool attachTesselationControlShader(const char * source) = 0;
	virtual bool attachTesselationEvaluationShader(const char * source) = 0;
	virtual bool attachFragmentShader(const char * source) = 0;
	virtual bool compile() = 0;
};

#endif // ishader_h__