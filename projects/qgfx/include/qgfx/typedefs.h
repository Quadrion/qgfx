#ifndef typedefs_h__
#define typedefs_h__

#if defined(QGFX_OPENGL)
class OpenGLPipeline;
class OpenGLRasterizer;
class OpenGLShader;
class OpenGLVertexBuffer;
class OpenGLCommandPool;
class OpenGLCommandBuffer;

using Pipeline = OpenGLPipeline;
using Rasterizer = OpenGLRasterizer;
using Shader = OpenGLShader;
using VertexBuffer = OpenGLVertexBuffer;
using CommandPool = OpenGLCommandPool;
using CommandBuffer = OpenGLCommandBuffer;
#elif defined(QGFX_VULKAN)
class VulkanPipeline;
class VulkanRasterizer;
class VulkanShader;
class VulkanVertexBuffer;
class VulkanCommandPool;
class VulkanCommandBuffer;

using Pipeline = VulkanPipeline;
using Rasterizer = VulkanRasterizer;
using Shader = VulkanShader;
using VertexBuffer = VulkanVertexBuffer;
using CommandPool = VulkanCommandPool;
using CommandBuffer = VulkanCommandBuffer;
#endif

#endif // typedefs_h__