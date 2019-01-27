#if defined(QGFX_OPENGL)

#include "qgfx/opengl/opengl_pipeline.h"

#include <glad/glad.h>

OpenGLPipeline::OpenGLPipeline(ContextHandle * handle)
	: IPipeline(handle)
{
}

OpenGLPipeline::~OpenGLPipeline()
{
	// no op
}

void OpenGLPipeline::addShader(const qtl::shared_ptr<Shader>& shader)
{
	mShaders.push_back(shader);
}

void OpenGLPipeline::construct()
{
}

void OpenGLPipeline::setTopology(const Topology & topology)
{
	// no op
}

#endif