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

void OpenGLPipeline::setTopology(const Topology & topology)
{
	// no op
}

#endif