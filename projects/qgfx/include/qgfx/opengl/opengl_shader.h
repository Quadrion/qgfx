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

		bool attachVertexShader(const qtl::vector<char>& source) override;
		bool attachGeometryShader(const qtl::vector<char>& source) override;
		bool attachTesselationControlShader(const qtl::vector<char>& source) override;
		bool attachTesselationEvaluationShader(const qtl::vector<char>& source) override;
		bool attachFragmentShader(const qtl::vector<char>& source) override;
		bool compile() override;

		bool bind() override;
		bool unbind() override;
		uint32_t getStageCount() const override;
		qtl::vector<void*> getStages() const override;
	private:
		GLuint mId;
		qtl::tree_map<GLenum, GLuint> mStages;

		bool _createStage(const qtl::vector<char>& src, GLenum type);
};

#endif // opengl_shader_h__