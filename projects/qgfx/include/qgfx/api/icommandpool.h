#ifndef icommandpool_h__
#define icommandpool_h__

class ICommandPool
{
	public:
		ICommandPool() = default;
		virtual ~ICommandPool() = default;

		ICommandPool& operator = (const ICommandPool&) = delete;

		
};

#endif // icommandpool_h__