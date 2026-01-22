#pragma once
#include "GameplayAttribute.h"

#include <vector>

namespace sm
{
	struct Modifier;
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

		void AddModifier(AttributeID id, const sm::Modifier& mod);

	private:
		std::vector<sm::GameplayAttribute> m_Attributes;
	};
}