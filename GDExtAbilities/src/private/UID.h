#include "Types.h"

namespace sm
{
	class UID
	{
	public:
		uint32 GenerateUID()
		{
			static std::atomic<uint32> nextID{ 1 };
			return nextID++;
		}

	public:
		uint32 nextID = 0;
	};
}