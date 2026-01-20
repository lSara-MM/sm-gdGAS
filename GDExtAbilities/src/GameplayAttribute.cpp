#include "GameplayAttribute.h"

#include <algorithm>

void sm::GameplayAttribute::Calculate()
{
	
}

void sm::GameplayAttribute::Reset()
{
	m_CurrentValue = std::clamp(m_BaseValue, m_MinValue, m_MaxValue);

	m_dirty = false;
}
