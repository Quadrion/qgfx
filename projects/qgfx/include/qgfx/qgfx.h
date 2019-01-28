/*
	Copyright (c) 2019 Jonathan Bastnagel, Renato Ciuffo, Nicholaus Clark, Roderick Griffioen, and Shawn Simonson

	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
	to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
	and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
	DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef qgfx_h__
#define qgfx_h__

#if defined(QGFX_OPENGL)
#include <glad/glad.h>
#endif

#include "qgfx/context_handle.h"
#include "qgfx/shader_loader.h"
#include "qgfx/qassert.h"

#include "qgfx/typedefs.h"

#if defined(QGFX_OPENGL)
#include "qgfx/opengl/opengl_pipeline.h"
#include "qgfx/opengl/opengl_rasterizer.h"
#include "qgfx/opengl/opengl_shader.h"
#include "qgfx/opengl/opengl_vertexbuffer.h"
#include "qgfx/opengl/opengl_commandbuffer.h"
#include "qgfx/opengl/opengl_commandpool.h"
#elif defined(QGFX_VULKAN)
#include "qgfx/vulkan/vulkan_pipeline.h"
#include "qgfx/vulkan/vulkan_rasterizer.h"
#include "qgfx/vulkan/vulkan_shader.h"
#include "qgfx/vulkan/vulkan_vertexbuffer.h"
#include "qgfx/vulkan/vulkan_commandbuffer.h"
#include "qgfx/vulkan/vulkan_commandpool.h"
#include "qgfx/vulkan/vulkan_window.h"
#endif

#endif // qgfx_h__