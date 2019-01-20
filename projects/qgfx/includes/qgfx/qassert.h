#ifndef qassert_h__
#define qassert_h__

#include <cassert>
#include <cstdio>
#include <cstdarg>

inline void qgfx_assert_msg_impl(bool x, const char* format, ...)
{
	if(!x)
	{
		va_list va;
		va_start(va, format);
		vprintf(format, va);
		va_end(va);

		assert(x);
	}
}

#if defined(_DEBUG)
	#define QGFX_ASSERT(x) assert(x)
	#define QGFX_ASSERT_MSG(x, format, ...) qgfx_assert_msg_impl(x, format, ##__VA_ARGS__)
	#define QGFX_STATIC_ASSERT(x) static_assert(x)
#else
	#define QGFX_ASSERT(x)
	#define QGFX_ASSERT_MSG(x, format, ...)
	#define QGFX_STATIC_ASSERT(x)
#endif

#endif // qassert_h__