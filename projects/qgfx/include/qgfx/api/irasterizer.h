#ifndef irasterizer_h__
#define irasterizer_h__

#include <stdint.h>

enum class CullMode : int32_t
{
	Front,
	Back,
	FrontAndBack
};

enum class FrontFace : int32_t
{
	Clockwise,
	CounterClockwise
};

enum class PolygonMode : int32_t
{
	Point,
	Line,
	Fill
};

class IRasterizer
{
	public:
		IRasterizer() = default;
		virtual ~IRasterizer() = default;

		IRasterizer& operator = (const IRasterizer&) = delete;

		virtual void setCullMode(const CullMode mode) = 0;
		virtual void setFrontFace(const FrontFace face) = 0;

		virtual void setPolygonMode(const PolygonMode mode, const CullMode face) = 0;

		virtual void setLineWidth(const float lineWidth) = 0;

		virtual void setDepthTest(const bool enabled) = 0;
};

#endif // irasterizer_h__