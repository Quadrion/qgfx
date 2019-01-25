#ifndef opengl_pipeline_h__
#define opengl_pipeline_h__

#include "qgfx/api/ipipeline.h"

class OpenGLPipeline : public IPipeline
{
	public:
		OpenGLPipeline();
		~OpenGLPipeline();

		OpenGLPipeline& operator=(const OpenGLPipeline&) = delete;
	
		void setTopology(const Topology& topology) override;
};

#endif // opengl_pipeline_h__