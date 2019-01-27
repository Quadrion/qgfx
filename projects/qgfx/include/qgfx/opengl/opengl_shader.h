#ifndef opengl_shader_h__
#define opengl_shader_h__

#include <glad/glad.h>
#include <qtl/tree_map.h>

#include "qgfx/api/ishader.h"
#include "qgfx/context_handle.h"

class OpenGLShader : public IShader
{
	public:
		explicit OpenGLShader(ContextHandle* handle);
		OpenGLShader(const OpenGLShader&) = delete;
		OpenGLShader(OpenGLShader&& shader) noexcept;
		~OpenGLShader() override;

		OpenGLShader& operator=(const OpenGLShader&) = delete;
		OpenGLShader& operator=(OpenGLShader&& shader) noexcept;

		bool attachVertexShader(const qtl::string& source) override;
		bool attachGeometryShader(const qtl::string& source) override;
		bool attachTesselationControlShader(const qtl::string& source) override;
		bool attachTesselationEvaluationShader(const qtl::string& source) override;
		bool attachFragmentShader(const qtl::string& source) override;
		bool compile() override;

		bool bind() override;
		bool unbind() override;
	private:
		GLuint mId;
		qtl::tree_map<GLenum, GLuint> mStages;

		bool _createStage(const qtl::string& src, GLenum type);
};

#endif // opengl_shader_h__