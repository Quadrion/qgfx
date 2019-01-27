#ifndef opengl_pipeline_h__
#define opengl_pipeline_h__

#include "qgfx/api/ipipeline.h"

#include <qtl/vector.h>

class OpenGLPipeline : public IPipeline
{
	public:
		OpenGLPipeline(ContextHandle* handle);
		OpenGLPipeline(const OpenGLPipeline&) = delete;
		~OpenGLPipeline();

		OpenGLPipeline& operator=(const OpenGLPipeline&) = delete;
	
		void addShader(const qtl::shared_ptr<Shader>& shader) override;
		void construct() override;
		void setTopology(const Topology& topology) override;
	private:
		qtl::vector<qtl::shared_ptr<Shader>> mShaders;
};

#endif // opengl_pipeline_h__