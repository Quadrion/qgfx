#ifndef icontexthandle_h__
#define icontexthandle_h__

#include "qgfx/typedefs.h"

#include "GLFW/glfw3.h"

class IContextHandle
{
	public:
		explicit IContextHandle(GLFWwindow* window);
		virtual ~IContextHandle() = default;

		IContextHandle& operator = (const IContextHandle&) = delete;

		virtual Pipeline* getPipeline() const = 0;
		virtual Rasterizer* getRasterizer() const = 0;

		virtual void initializeGraphics() = 0;
		virtual void finalizeGraphics() = 0;

	protected:
		GLFWwindow* mWindow;
};

#endif // icontexthandle_h__