#ifndef opengl_rasterizer_h__
#define opengl_rasterizer_h__

#include <glad/glad.h>

#include "qgfx/api/irasterizer.h"

class OpenGLRasterizer : public IRasterizer
{
	public:
		OpenGLRasterizer(ContextHandle* handle);
		OpenGLRasterizer(const OpenGLRasterizer&) = delete;
		~OpenGLRasterizer();

		void setCullMode(const CullMode mode) override;
		void setFrontFace(const FrontFace face) override;
		void setPolygonMode(const PolygonMode mode, const CullMode face) override;
		void setLineWidth(const float lineWidth) override;
		void setDepthTest(const bool enabled) override;
	private:
		CullMode mCulling = CullMode::Back;
		FrontFace mFace = FrontFace::CounterClockwise;
		PolygonMode mFillMode = PolygonMode::Fill;
		CullMode mFillFace = CullMode::FrontAndBack;
		float mLineWidth = 1.0f;
		bool mDepthEnabled = false;
};

#endif // opengl_rasterizer_h__