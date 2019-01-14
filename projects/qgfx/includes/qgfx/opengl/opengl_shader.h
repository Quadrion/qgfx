#ifndef opengl_shader_h__
#define opengl_shader_h__

#include <glad/glad.h>
#include "qgfx/api/ishader.h"

#include <map>
#include "qgfx/context_handle.h"

class OpenGLShader : public IShader
{
	public:
		explicit OpenGLShader(ContextHandle* handle);
		~OpenGLShader() override;

		OpenGLShader& operator=(const OpenGLShader&) = delete;

		bool attachVertexShader(const std::string& source) override;
		bool attachGeometryShader(const std::string& source) override;
		bool attachTesselationControlShader(const std::string& source) override;
		bool attachTesselationEvaluationShader(const std::string& source) override;
		bool attachFragmentShader(const std::string& source) override;
		bool compile() override;

		bool bind() override;
		bool unbind() override;
	private:
		GLuint mId;
		std::map<GLenum, GLuint> mStages;

		bool _createStage(const std::string& src, GLenum type);
};

#endif // opengl_shader_h__