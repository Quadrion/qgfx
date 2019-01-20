#ifndef ipipeline_h__
#define ipipeline_h__

enum class Topology : int32_t
{
	TriangleList,
	TriangleStrip,
	Line,
	Quads,
	Points
};

class IPipeline
{
	public:
		IPipeline() = default;
		virtual ~IPipeline() = 0;

		IPipeline& operator = (const IPipeline&) = delete;

		virtual void setTopology(const Topology& topology) = 0;
};

#endif // ipipeline_h__