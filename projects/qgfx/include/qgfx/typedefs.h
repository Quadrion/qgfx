#ifndef typedefs_h__
#define typedefs_h__

#if defined(QGFX_OPENGL)
class OpenGLPipeline;
class OpenGLRasterizer;
class OpenGLShader;
class OpenGLVertexBuffer;
class OpenGLCommandPool;
class OpenGLCommandBuffer;
class OpenGLWindow;
class OpenGLImage2D;

using Pipeline = OpenGLPipeline;
using Rasterizer = OpenGLRasterizer;
using Shader = OpenGLShader;
using VertexBuffer = OpenGLVertexBuffer;
using CommandPool = OpenGLCommandPool;
using CommandBuffer = OpenGLCommandBuffer;
using Window = OpenGLWindow;
using Image2D = OpenGLImage2D;
#elif defined(QGFX_VULKAN)
class VulkanPipeline;
class VulkanRasterizer;
class VulkanShader;
class VulkanVertexBuffer;
class VulkanCommandPool;
class VulkanCommandBuffer;
class VulkanWindow;
class VulkanRenderPass;
class VulkanImage2D;

using Pipeline = VulkanPipeline;
using Rasterizer = VulkanRasterizer;
using Shader = VulkanShader;
using VertexBuffer = VulkanVertexBuffer;
using CommandPool = VulkanCommandPool;
using CommandBuffer = VulkanCommandBuffer;
using Window = VulkanWindow;
using RenderPass = VulkanRenderPass;
using Image2D = VulkanImage2D;
#endif

#endif // typedefs_h__