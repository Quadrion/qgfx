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
	
		Shader* addShader() override;
		void construct() override;
		void setTopology(const Topology& topology) override;
	private:
		qtl::vector<Shader*> mShaders;
};

#endif // opengl_pipeline_h__