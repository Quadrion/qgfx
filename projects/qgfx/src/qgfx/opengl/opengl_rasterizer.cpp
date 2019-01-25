#include "..\..\..\include\qgfx\opengl\opengl_rasterizer.h"
#if defined(QGFX_OPENGL)

OpenGLRasterizer::OpenGLRasterizer()
{
}

OpenGLRasterizer::~OpenGLRasterizer()
{
}

void OpenGLRasterizer::setCullMode(const CullMode mode)
{
	if (mode == mCulling)
	{
		return;
	}
	mCulling = mode;
	switch (mode)
	{
		case CullMode::Front:
		{
			glCullFace(GL_FRONT);
			break;
		}
		case CullMode::Back:
		{
			glCullFace(GL_BACK);
			break;
		}
		case CullMode::FrontAndBack:
		{	
			glCullFace(GL_FRONT_AND_BACK); 
			break; 
		}
	}
}

void OpenGLRasterizer::setFrontFace(const FrontFace face)
{
	if (face == mFace)
	{
		return;
	}
	mFace = face;
	switch (face)
	{
		case FrontFace::Clockwise:
		{
			glFrontFace(GL_CW);
			break;
		}
		case FrontFace::CounterClockwise:
		{
			glFrontFace(GL_CCW);
			break;
		}
	}
}

void OpenGLRasterizer::setPolygonMode(const PolygonMode mode, const CullMode face)
{
	GLenum pmode = GL_FILL;
	GLenum pface = GL_FRONT_AND_BACK;

	if (mode == mFillMode && face == mFillFace)
	{
		return;
	}

	switch (face)
	{
		case CullMode::Front:
		{
			pface = GL_FRONT;
			break;
		}
		case CullMode::Back:
		{
			pface = GL_BACK;
			break;
		}
		case CullMode::FrontAndBack:
		{
			pface = GL_FRONT_AND_BACK;
			break;
		}
	}

	switch (mode)
	{
		case PolygonMode::Fill:
		{
			pmode = GL_FILL;
			break;
		}
		case PolygonMode::Line:
		{
			pmode = GL_LINE;
			break;
		}
		case PolygonMode::Point:
		{
			pmode = GL_POINT;
			break;
		}
	}
	
	mFillFace = face;
	mFillMode = mode;
	glPolygonMode(pface, pmode);
}

void OpenGLRasterizer::setLineWidth(const float lineWidth)
{
	mLineWidth = lineWidth;
	glLineWidth(lineWidth);
}

void OpenGLRasterizer::setDepthTest(const bool enabled)
{
	if (mDepthEnabled == enabled)
	{
		return;
	}
	mDepthEnabled = enabled;
	if (enabled)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

#endif
