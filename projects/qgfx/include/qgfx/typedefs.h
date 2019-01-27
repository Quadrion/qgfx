#ifndef typedefs_h__
#define typedefs_h__

#if defined(QGFX_OPENGL)
class OpenGLPipeline;
class OpenGLRasterizer;
class OpenGLShader;
class OpenGLVertexBuffer;

using Pipeline = OpenGLPipeline;
using Rasterizer = OpenGLRasterizer;
using Shader = OpenGLShader;
using VertexBuffer = OpenGLVertexBuffer;
#elif defined(QGFX_VULKAN)
class VulkanPipeline;
class VulkanRasterizer;
class VulkanShader;
class VulkanVertexBuffer;

using Pipeline = VulkanPipeline;
using Rasterizer = VulkanRasterizer;
using Shader = VulkanShader;
using VertexBuffer = VulkanVertexBuffer;
#endif

#endif // typedefs_h__