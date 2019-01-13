#ifndef context_handle_h__
#define context_handle_h__

#if defined(QGFX_OPENGL)
using ContextHandle = void*;
#elif defined(QGFX_VULKAN)
#include "qgfx/vulkan/vulkan_context_handle.h"
using ContextHandle = VulkanContextHandle;
#endif

#endif // context_handle_h__