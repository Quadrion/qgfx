#include "qgfx/qgfx.h"
#include "GLFW/glfw3.h"

#include <qtl/string.h>

int main()
{
#if defined(QGFX_OPENGL)
	/* Initialize the library */
	if (!glfwInit())
		return -1;
	
	auto ptext = load_text("media/effects/shader.vert");
	auto spirv = load_spirv("media/effects/vert.spv");

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

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	delete contextHandle;

	glfwDestroyWindow(window);
	glfwTerminate();

#endif

	return 0;
}
