#ifndef ishader_h__
#define ishader_h__

#include "qgfx/context_handle.h"

#include <qtl/vector.h>

class IShader
{
	public:
		explicit IShader(ContextHandle* handle);
		virtual ~IShader() = default;

		IShader& operator = (const IShader&) = delete;

		virtual bool attachVertexShader(const qtl::vector<char>& source) = 0;
		virtual bool attachGeometryShader(const qtl::vector<char>& source) = 0;
		virtual bool attachTesselationControlShader(const qtl::vector<char>& source) = 0;
		virtual bool attachTesselationEvaluationShader(const qtl::vector<char>& source) = 0;
		virtual bool attachFragmentShader(const qtl::vector<char>& source) = 0;
		virtual bool compile() = 0;
		virtual bool cleanup() = 0;

		virtual bool bind() = 0;
		virtual bool unbind() = 0;
		virtual uint32_t getStageCount() const = 0;
		virtual qtl::vector<void*> getStages() const = 0;
	protected:
		ContextHandle* mHandle;
};

#endif // ishader_h__