#include "GameplayAttributeSet.h"

sm::GameplayAttributeSet::GameplayAttributeSet()
{}

sm::GameplayAttributeSet::~GameplayAttributeSet()
{}

void sm::GameplayAttributeSet::AddAttribute(uint32 id, float base, float min, float max)
{
	m_Attributes.emplace_back(id, base, min, max);
}