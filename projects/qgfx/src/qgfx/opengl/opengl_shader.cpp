#if defined(QGFX_OPENGL)

#include "qgfx/opengl/opengl_shader.h"
#include "qgfx/qassert.h"

OpenGLShader::OpenGLShader(ContextHandle* handle)
	: IShader(handle)
{
	mId = glCreateProgram();
	QGFX_ASSERT_MSG(mId != 0, "Failed to create OpenGL Shader!\n");
}

OpenGLShader::OpenGLShader(OpenGLShader&& shader) noexcept
	: IShader(shader.mHandle), mId(shader.mId)
{
	shader.mId = 0;
}

OpenGLShader::~OpenGLShader()
{
	if (mId)
	{
		glDeleteProgram(mId);
		mId = 0;
	}
}

OpenGLShader& OpenGLShader::operator=(OpenGLShader&& shader) noexcept
{
	mId = shader.mId;
	shader.mId = 0;
	return *this;
}

bool OpenGLShader::attachVertexShader(const qtl::vector<char>& source)
{
	const bool res = _createStage(source, GL_VERTEX_SHADER);

	return res;
}

bool OpenGLShader::attachGeometryShader(const qtl::vector<char>& source)
{
	const bool res = _createStage(source, GL_GEOMETRY_SHADER);

	return res;
}

bool OpenGLShader::attachTesselationControlShader(const qtl::vector<char>& source)
{
	const bool res = _createStage(source, GL_TESS_CONTROL_SHADER);

	return res;
}

bool OpenGLShader::attachTesselationEvaluationShader(const qtl::vector<char>& source)
{
	const bool res = _createStage(source, GL_TESS_EVALUATION_SHADER);

	return res;
}

bool OpenGLShader::attachFragmentShader(const qtl::vector<char>& source)
{
	const bool res = _createStage(source, GL_FRAGMENT_SHADER);
	
	return res;
}

bool OpenGLShader::compile()
{
	for (const auto stage : mStages)
	{
		glAttachShader(mId, stage.second);
	}
	glLinkProgram(mId);

	GLint status;
	glGetProgramiv(mId, GL_LINK_STATUS, &status);
	if (!status)
	{
		GLint len;
		glGetProgramiv(mId, GL_INFO_LOG_LENGTH, &len);
		char * log = new char[static_cast<size_t>(len) + 1];
		glGetProgramInfoLog(mId, len, &len, log);
		QGFX_ASSERT_MSG(status, "%s\n", log);
		delete[] log;
		return false;
	}
	glValidateProgram(mId);
	glGetProgramiv(mId, GL_VALIDATE_STATUS, &status);
	if (!status)
	{
		GLint len;
		glGetProgramiv(mId, GL_INFO_LOG_LENGTH, &len);
		char * log = new char[static_cast<size_t>(len) + 1];
		glGetProgramInfoLog(mId, len, &len, log);
		QGFX_ASSERT_MSG(status, "%s\n", log);
		delete[] log;
		return false;
	}

	for (const auto stage : mStages)
	{
		glDeleteShader(stage.second);
	}

	return true;
}

bool OpenGLShader::cleanup()
{
	return true;
}

bool OpenGLShader::bind()
{
	glUseProgram(mId);

	return true;
}

bool OpenGLShader::unbind()
{
	glUseProgram(0);

	return true;
}

uint32_t OpenGLShader::getStageCount() const
{
	return static_cast<uint32_t>(mStages.size());
}

qtl::vector<void*> OpenGLShader::getStages() const
{
	return qtl::vector<void*>();
}

bool OpenGLShader::_createStage(const qtl::vector<char>& src, const GLenum type)
{
	const GLuint stage = glCreateShader(type);
	const char* source = src.data();
	glShaderSource(stage, 1, &source, nullptr);
	glCompileShader(stage);

	GLint success;
	glGetShaderiv(stage, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLint len;
		glGetShaderiv(stage, GL_INFO_LOG_LENGTH, &len);
		char* log = new char[len];
		glGetShaderInfoLog(stage, len, nullptr, log);
		QGFX_ASSERT_MSG(success, "%s\n", log);
		delete[] log;
	}
	else
	{
		mStages.insert({ type, stage });
	}

	return success == GL_TRUE;
}

#endif // QGFX_OPENGL