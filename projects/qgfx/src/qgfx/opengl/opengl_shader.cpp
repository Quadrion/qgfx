#include "qgfx/opengl/opengl_shader.h"
#include "qgfx/qassert.h"

#include <cstring>

OpenGLShader::OpenGLShader()
{
	__id = glCreateProgram();
	QGFX_ASSERT_MSG(__id != 0, "Failed to create OpenGL Shader!\n");
}

OpenGLShader::~OpenGLShader()
{
	if (__id)
	{
		glDeleteProgram(__id);
		__id = 0;
	}
}

bool OpenGLShader::attachVertexShader(const char * source)
{
	size_t len = strlen(source) + 1;
	char * cpy = new char[len];

	memcpy(cpy, source, len);

	bool res = __create_stage(source, GL_VERTEX_SHADER);

	delete[] cpy;

	return res;
}

bool OpenGLShader::attachGeometryShader(const char * source)
{
	size_t len = strlen(source) + 1;
	char * cpy = new char[len];

	memcpy(cpy, source, len);

	bool res = __create_stage(source, GL_GEOMETRY_SHADER);

	delete[] cpy;

	return res;
}

bool OpenGLShader::attachTesselationControlShader(const char * source)
{
	size_t len = strlen(source) + 1;
	char * cpy = new char[len];

	memcpy(cpy, source, len);

	bool res = __create_stage(source, GL_TESS_CONTROL_SHADER);

	delete[] cpy;

	return res;
}

bool OpenGLShader::attachTesselationEvaluationShader(const char * source)
{
	size_t len = strlen(source) + 1;
	char * cpy = new char[len];

	memcpy(cpy, source, len);

	bool res = __create_stage(source, GL_TESS_EVALUATION_SHADER);

	delete[] cpy;

	return res;
}

bool OpenGLShader::attachFragmentShader(const char * source)
{
	size_t len = strlen(source) + 1;
	char * cpy = new char[len];

	memcpy(cpy, source, len);

	bool res = __create_stage(source, GL_FRAGMENT_SHADER);

	delete[] cpy;

	return res;
}

bool OpenGLShader::compile()
{
	
}

bool OpenGLShader::__create_stage(const char * src, GLenum type)
{
	GLuint stage = glCreateShader(type);
	glShaderSource(stage, 1, &src, NULL);
	glCompileShader(stage);

	GLint success;
	glGetShaderiv(stage, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLint len;
		glGetShaderiv(stage, GL_INFO_LOG_LENGTH, &len);
		char * log = new char[len];
		glGetShaderInfoLog(stage, len, NULL, log);
		QGFX_ASSERT_MSG(success, "%s\n", log);
		delete[] log;
	}
	else
	{
		__stages.insert(std::make_pair(type, stage));
	}

	return success != 0;
}
