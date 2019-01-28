#include "qgfx/qgfx.h"
#include "GLFW/glfw3.h"

#include <qtl/string.h>

int main()
{
#if defined(QGFX_OPENGL)
	/* Initialize the library */
	if (!glfwInit())
		return -1;


	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(1280, 720, "QGFX Window", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	ContextHandle* handle = new ContextHandle(window);
	const auto vertex = load_text("media/effects/shader.vert");
	const auto fragment = load_text("media/effects/shader.frag");

	qtl::shared_ptr<Shader> shader = qtl::make_shared<Shader>(handle);
	shader->attachVertexShader(vertex);
	shader->attachFragmentShader(fragment);
	shader->compile();
	handle->getPipeline()->addShader(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	delete handle;

	glfwTerminate();
#elif defined(QGFX_VULKAN)
	Window* window = new Window();
	window->construct(1280, 720, "QGFX");

	ContextHandle* contextHandle = new ContextHandle(window);

	// create shader and meshes
	const auto vs = load_spirv("media/effects/vert.spv");
	const auto fs = load_spirv("media/effects/frag.spv");

	Shader* shader = contextHandle->getPipeline()->addShader();
	shader->attachVertexShader(vs);
	shader->attachFragmentShader(fs);
	shader->compile();

	contextHandle->initializeGraphics();

	shader->cleanup();

	CommandPool* pool = contextHandle->addCommandPool();

	for (size_t i = 0; i < contextHandle->getSwapChainFramebuffers().size(); i++)
	{
		pool->addCommandBuffer();
	}
	pool->construct();

	for (size_t i = 0; i < contextHandle->getSwapChainFramebuffers().size(); i++)
	{
		CommandBuffer* cmdBuffer = pool->getBuffers()[i];
		cmdBuffer->record();

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = contextHandle->getPipeline()->getRenderPass();
		renderPassInfo.framebuffer = contextHandle->getSwapChainFramebuffers()[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = contextHandle->getSwapChainExtent();

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(cmdBuffer->getBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(cmdBuffer->getBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, contextHandle->getPipeline()->getPipeline());
		vkCmdDraw(cmdBuffer->getBuffer(), 3, 1, 0, 0);

		vkCmdEndRenderPass(cmdBuffer->getBuffer());

		cmdBuffer->end();
	}

	contextHandle->finalizeGraphics();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(static_cast<GLFWwindow*>(window->getPlatformHandle())))
	{
		glfwPollEvents();

		contextHandle->startFrame();

		contextHandle->endFrame();
		contextHandle->swap();
	}

	vkDeviceWaitIdle(contextHandle->getLogicalDevice());

	delete contextHandle;

	glfwDestroyWindow(static_cast<GLFWwindow*>(window->getPlatformHandle()));
	glfwTerminate();

#endif

	return 0;
}
