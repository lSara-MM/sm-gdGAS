#include "GameplayAttribute.h"

#include <algorithm>

void sm::GameplayAttribute::Calculate()
{
	if (!m_dirty)
	{
		return;
	}

	//for (const sm::GameplayModifier& mod : m_Modifiers)
	//{

	//}
}

void sm::GameplayAttribute::AddModifier(const sm::GameplayModifier& mod)
{
	//m_Modifiers.emplace_back(mod);
	m_dirty = true;
}

void sm::GameplayAttribute::Reset()
{
	m_CurrentValue = std::clamp(m_BaseValue, m_MinValue, m_MaxValue);
	m_dirty = false;
}