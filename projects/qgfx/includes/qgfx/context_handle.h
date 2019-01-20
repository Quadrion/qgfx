#ifndef context_handle_h__
#define context_handle_h__

#undef QGFX_OPENGL

#if defined(QGFX_OPENGL)
#include "qgfx/opengl/opengl_context_handle.h"
using ContextHandle = OpenGLContextHandle;
#elif defined(QGFX_VULKAN)
#include "qgfx/vulkan/vulkan_context_handle.h"
using ContextHandle = VulkanContextHandle;
#endif

#endif // context_handle_h__