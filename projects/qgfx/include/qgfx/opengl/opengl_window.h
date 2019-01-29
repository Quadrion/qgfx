#ifndef openglwindow_h__
#define openglwindow_h__

#include "GLFW/glfw3.h"

#include "qgfx/api/iwindow.h"

class OpenGLWindow : public IWindow
{
    public:
	    OpenGLWindow() = default;
	    ~OpenGLWindow();

	    void construct(const WindowCreationParameters& params) override;
	    void construct(const uint32_t width, const uint32_t height, const qtl::string& title, const bool fullscreen = false, const bool vsync = false) override;
	    void* getPlatformHandle() const override;;
	    bool shouldClose() const override;
	    void poll() const override;
    private:
	    GLFWwindow* mHandle = nullptr;
};

#endif