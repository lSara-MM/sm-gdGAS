#include "GameplayAttributeSet.h"

#include "GameplayModifier.h"

sm::GameplayAttributeSet::GameplayAttributeSet()
{}

sm::GameplayAttributeSet::~GameplayAttributeSet()
{}

sm::GameplayAttribute* sm::GameplayAttributeSet::FindAttribute(AttributeID id)
{
	// AttributeSets are small (< 100 attributes), linear search is acceptable
	for (sm::GameplayAttribute& attr : m_Attributes)
	{
		if (attr.GetUID() == id)
		{
			return &attr;
		}
	}

	return nullptr;
}

sm::GameplayAttribute& sm::GameplayAttributeSet::AddAttribute(AttributeID id, float base, float min, float max)
{
	return m_Attributes.emplace_back(id, base, min, max);
}

void sm::GameplayAttributeSet::AddModifier(AttributeID id, const sm::GameplayModifier& mod)
{
	/*if (sm::GameplayAttribute* attr = FindAttribute(id); attr != nullptr)
	{
		attr.
	}*/
}
