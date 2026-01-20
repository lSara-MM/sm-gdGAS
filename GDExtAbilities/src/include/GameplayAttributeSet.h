#pragma once
#include "GameplayAttribute.h"

#include <vector>

namespace sm
{
	class GameplayAttributeSet
	{
	public:
		GameplayAttributeSet();
		~GameplayAttributeSet();

		//GameplayAttribute& GetAttribute(uint32 id) const;
		void AddAttribute(uint32 id, float base, float min = 0.0f, float max = FLT_MAX);

	private:
		std::vector<GameplayAttribute> m_Attributes;
	};
}