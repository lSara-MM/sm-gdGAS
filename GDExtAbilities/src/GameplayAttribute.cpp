#include "GameplayAttribute.h"

#include "gdModifier.h"

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

void sm::GameplayAttribute::SetBase(float newValue)
{
	m_BaseValue = std::clamp(newValue, m_MinValue, m_MaxValue);
}

sm::GameplayModifier* sm::GameplayAttribute::FindModifier(godot::Ref<sm::Modifier> mod)
{
	for (sm::GameplayModifier& modifier : m_Modifiers)
	{
		if ((int)mod->operation == (int)modifier.operation && mod->sourceID == modifier.sourceID)
		{
			return &modifier;
		}
	}
	
	return nullptr;
}

void sm::GameplayAttribute::AddModifier(const godot::Ref<sm::Modifier> mod)
{
	sm::GameplayModifier modifier {
		m_ModifiersUID.GenerateUID(),
		(GameplayModifier::OperationType)mod->operation,
		mod->targetID,
		mod->sourceID,
		mod->value
	};

	m_Modifiers.push_back(modifier);
	m_dirty = true;
}

void sm::GameplayAttribute::Reset()
{
	m_CurrentValue = std::clamp(m_BaseValue, m_MinValue, m_MaxValue);
	m_dirty = false;
}