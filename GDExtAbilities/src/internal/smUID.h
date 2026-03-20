#pragma once
#include "internal/Types.h"

namespace sm
{
	class DumbUID
	{
	public:
		uint32 GenerateUID()
		{
			return ++nextID;
		}

		// Use carefully
		uint32 GetCurrentID()
		{
			return nextID;
		}

	private:
		uint32 nextID = 0;
	};
}