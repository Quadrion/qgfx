#if defined(QGFX_OPENGL)

#include "qgfx/opengl/opengl_pipeline.h"

#include <glad/glad.h>

OpenGLPipeline::OpenGLPipeline(ContextHandle* handle)
	: IPipeline(handle)
{
}

OpenGLPipeline::OpenGLPipeline(OpenGLPipeline&& pipeline) noexcept
	: IPipeline(pipeline.mHandle), mShaders(qtl::move(pipeline.mShaders))
{
	pipeline.mShaders.clear();
}

OpenGLPipeline::~OpenGLPipeline()
{
	mShaders.clear();
}

OpenGLPipeline & OpenGLPipeline::operator=(OpenGLPipeline&& pipeline) noexcept
{
	mHandle = pipeline.mHandle;
	mShaders = qtl::move(pipeline.mShaders);
	pipeline.mShaders.clear();
	return *this;
}

void OpenGLPipeline::addShader(const qtl::shared_ptr<Shader>& shader)
{
	mShaders.push_back(shader);
}

void OpenGLPipeline::construct()
{
}

void OpenGLPipeline::setTopology(const Topology& topology)
{
	// no op
}

#endif