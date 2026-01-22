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
		sm::GameplayAttribute& AddAttribute(AttributeID id, float base, float min = 0.0f, float max = FLT_MAX);

		void AddModifier(AttributeID id, const sm::GameplayModifier& mod);

	private:
		std::vector<sm::GameplayAttribute> m_Attributes;
	};
}