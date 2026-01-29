#pragma once
#include "GameplayAttribute.h"

#include <vector>

namespace sm
{
	struct GameplayModifier;
}

namespace sm
{
	class GameplayAttributeSet
	{
	public:
		GameplayAttributeSet();
		~GameplayAttributeSet();

		GameplayAttribute* FindAttribute(AttributeID id);
		GameplayAttribute& AddAttribute(AttributeID id, float base, float min = 0.0f, float max = FLT_MAX);

	private:
		std::vector<GameplayAttribute> m_Attributes;
	};
}