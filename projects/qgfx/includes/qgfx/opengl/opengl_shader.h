#ifndef opengl_shader_h__
#define opengl_shader_h__

#include "glad/glad.h"
#include "qgfx/api/ishader.h"

#include <map>

class OpenGLShader : public IShader
{
public:
	OpenGLShader();
	~OpenGLShader() override;

	OpenGLShader& operator=(const OpenGLShader&) = delete;

	bool attachVertexShader(const char * source) override;
	bool attachGeometryShader(const char * source) override;
	bool attachTesselationControlShader(const char * source) override;
	bool attachTesselationEvaluationShader(const char * source) override;
	bool attachFragmentShader(const char * source) override;
	bool compile() override;
private:
	GLuint __id;
	std::map<GLenum, GLuint> __stages;

	bool __create_stage(const char * src, GLenum type);
};

#endif // opengl_shader_h__