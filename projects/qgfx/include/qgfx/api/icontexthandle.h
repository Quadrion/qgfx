#ifndef icontexthandle_h__
#define icontexthandle_h__

#include "qgfx/typedefs.h"

#include "GLFW/glfw3.h"

class IContextHandle
{
	public:
		explicit IContextHandle(Window* window);
		virtual ~IContextHandle() = default;

		IContextHandle& operator = (const IContextHandle&) = delete;

		virtual Pipeline* getPipeline() const = 0;
		virtual Rasterizer* getRasterizer() const = 0;

		virtual void initializeGraphics() = 0;
		virtual void finalizeGraphics() = 0;

		virtual CommandPool* addCommandPool() = 0;

		virtual void startFrame() = 0;
		virtual void endFrame() = 0;

		virtual void swap() = 0;

	protected:
		Window* mWindow;
};

#endif // icontexthandle_h__