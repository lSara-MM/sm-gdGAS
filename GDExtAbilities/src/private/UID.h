#pragma once
#include "Types.h"

namespace sm
{
	class DumbUID
	{
	public:
		uint32 GenerateUID()
		{
			return nextID++;
		}

	public:
		uint32 nextID = 0;
	};
}