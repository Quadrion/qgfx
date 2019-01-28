#ifndef opengl_pipeline_h__
#define opengl_pipeline_h__

#include "qgfx/api/ipipeline.h"
#include "qgfx/opengl/opengl_shader.h"

#include <qtl/vector.h>

class OpenGLPipeline : public IPipeline
{
	public:
		OpenGLPipeline(ContextHandle* handle);
		OpenGLPipeline(const OpenGLPipeline&) = delete;
		OpenGLPipeline(OpenGLPipeline&& pipeline) noexcept;
		~OpenGLPipeline();

		OpenGLPipeline& operator=(const OpenGLPipeline&) = delete;
		OpenGLPipeline& operator=(OpenGLPipeline&& pipeline) noexcept;
	
		void addShader(const qtl::shared_ptr<Shader>& shader) override;
		void construct() override;
		void setTopology(const Topology& topology) override;
	private:
		qtl::vector<qtl::shared_ptr<OpenGLShader>> mShaders;
};

#endif // opengl_pipeline_h__