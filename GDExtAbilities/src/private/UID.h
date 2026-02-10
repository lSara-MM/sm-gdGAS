#pragma once
#include "Types.h"

namespace sm
{
	class DumbUID
	{
	public:
		uint16 GenerateUID()
		{
			static std::atomic<uint16> nextID{ 1 };
			return nextID++;
		}

	public:
		uint16 nextID = 0;
	};
}