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
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(1280, 720, "QGFX Window", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	ContextHandle* contextHandle = new ContextHandle(window);

	// create shader and meshes
	const auto vs = load_spirv("media/effects/vert.spv");
	const auto fs = load_spirv("media/effects/frag.spv");

	qtl::shared_ptr<Shader> shader = qtl::make_shared<Shader>(contextHandle);
	shader->attachVertexShader(vs);
	shader->attachFragmentShader(fs);
	shader->compile();

	contextHandle->getPipeline()->addShader(shader);

	contextHandle->initializeGraphics();

	CommandPool* pool = new CommandPool(contextHandle);

	qtl::vector<qtl::shared_ptr<CommandBuffer>> bufs;

	for (size_t i = 0; i < contextHandle->getSwapChainFramebuffers().size(); i++)
	{
		const qtl::shared_ptr<CommandBuffer> cmdBuffer = qtl::make_shared<CommandBuffer>(contextHandle, pool);
		bufs.push_back(cmdBuffer);

		pool->addCommandBuffer(cmdBuffer);

		pool->construct();
	}

	for (size_t i = 0; i < contextHandle->getSwapChainFramebuffers().size(); i++)
	{
		const qtl::shared_ptr<CommandBuffer> cmdBuffer = bufs[i];
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

	int32_t currentFrame = 0;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		vkWaitForFences(contextHandle->getLogicalDevice(), 1, &contextHandle->getFences()[currentFrame], 
			VK_TRUE, std::numeric_limits<uint64_t>::max());
		vkResetFences(contextHandle->getLogicalDevice(), 1, &contextHandle->getFences()[currentFrame]);

		uint32_t imageIndex;
		vkAcquireNextImageKHR(contextHandle->getLogicalDevice(), contextHandle->getSwapChain(), 
			std::numeric_limits<uint64_t>::max(), contextHandle->getImageSemaphore()[currentFrame], VK_NULL_HANDLE, &imageIndex);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { contextHandle->getImageSemaphore()[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		VkCommandBuffer buffers[] = { bufs[imageIndex]->getBuffer() };
		submitInfo.pCommandBuffers = buffers;

		VkSemaphore signalSemaphores[] = { contextHandle->getRenderSemaphore()[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		const VkResult result = vkQueueSubmit(contextHandle->getGraphicsQueue(), 1, &submitInfo, 
			contextHandle->getFences()[currentFrame]);
		QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to submit draw command buffer!");

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { contextHandle->getSwapChain() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;

		vkQueuePresentKHR(contextHandle->getPresentQueue(), &presentInfo);

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	vkDeviceWaitIdle(contextHandle->getLogicalDevice());

	delete contextHandle;

	glfwDestroyWindow(window);
	glfwTerminate();

#endif

	return 0;
}
