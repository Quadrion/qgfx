#ifndef ipipeline_h__
#define ipipeline_h__

class IPipeline
{
	public:
		IPipeline() = default;
		virtual ~IPipeline() = 0;

		IPipeline& operator = (const IPipeline&) = delete;
};

#endif // ipipeline_h__