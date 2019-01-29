#include "qgfx/qgfx.h"

int main()
{
#if defined(QGFX_OPENGL)
	/* Create a windowed mode window and its OpenGL context */
	Window * win = new Window;
	win->construct(1280, 720, "QGFX WINDOW", false, false);

	/* Make the window's context current */

	ContextHandle* handle = new ContextHandle(win);
	handle->addCommandPool();
	const auto vertex = loadText("media/effects/shader.vert");
	const auto fragment = loadText("media/effects/shader.frag");

	auto shader = handle->getPipeline()->addShader();
	shader->attachVertexShader(vertex);
	shader->attachFragmentShader(fragment);
	shader->compile();

	/* Loop until the user closes the window */
	while (!win->shouldClose())
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		handle->swap();

		/* Poll for and process events */
		win->poll();
	}

	delete handle;

	glfwTerminate();
#elif defined(QGFX_VULKAN)
	Window* window = new Window();
	window->construct(1280, 720, "QGFX");

	ContextHandle* contextHandle = new ContextHandle(window);

	// create shader and meshes
	const auto vs = loadSpirv("media/effects/vert.spv");
	const auto fs = loadSpirv("media/effects/frag.spv");

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
	while (!window->shouldClose())
	{
		window->poll();

		contextHandle->startFrame();

		contextHandle->endFrame();
		contextHandle->swap();
	}

	vkDeviceWaitIdle(contextHandle->getLogicalDevice());

	delete contextHandle;

	delete window;

#endif

	return 0;
}
