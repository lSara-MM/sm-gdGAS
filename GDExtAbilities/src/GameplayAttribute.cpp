#include "GameplayAttribute.h"

#include "gdModifierData.h"

#include <algorithm>

void sm::GameplayAttribute::Calculate()
{
	if (!m_dirty)
	{
		return;
	}

	// Ignore all modifiers if there is an Override
	if (!m_Modifiers[static_cast<size_t>(ModifierOperationType::Override)].empty())
	{
		// If there are more than one, apply last
		auto& modifier = m_Modifiers[static_cast<size_t>(ModifierOperationType::Override)];

		m_CurrentValue = modifier[modifier.size() - 1].value;
		m_CurrentValue = std::clamp(m_CurrentValue, m_MinValue, m_MaxValue);

		m_dirty = false;
		return;
	}

	float sum = 0;
	for (auto& modifier : m_Modifiers[static_cast<size_t>(ModifierOperationType::Add)])
	{
		sum += modifier.value;
	}

	float mult = 1;
	for (auto& modifier : m_Modifiers[static_cast<size_t>(ModifierOperationType::Multiply)])
	{
		mult *= modifier.value;
	}

	float perAdd = 0;
	for (auto& modifier : m_Modifiers[static_cast<size_t>(ModifierOperationType::PercentAdd)])
	{
		perAdd += modifier.value;
	}

	float perStack = 1;
	for (auto& modifier : m_Modifiers[static_cast<size_t>(ModifierOperationType::PercentStack)])
	{
		perStack *= 1.0f + modifier.value * 0.01f;
	}

	//m_CurrentValue += sum;
	//m_CurrentValue *= mult;
	//m_CurrentValue += m_BaseValue * perAdd * 0.01f;
	//m_CurrentValue *= perStack;
	m_CurrentValue = ((m_BaseValue + sum) * mult + m_BaseValue * perAdd * 0.01f) * perStack;

	m_CurrentValue = std::clamp(m_CurrentValue, m_MinValue, m_MaxValue);

	m_dirty = false;
}

void sm::GameplayAttribute::SetBase(float newValue)
{
	m_BaseValue = std::clamp(newValue, m_MinValue, m_MaxValue);
}

float sm::GameplayAttribute::GetCurrent()
{
	if (m_dirty)
	{
		Calculate();
	}

	return m_CurrentValue;
}

size_t sm::GameplayAttribute::GetModifiersCount(ModifierOperationType op) const
{
	return m_Modifiers[static_cast<size_t>(op)].size();
}

sm::GameplayModifier* sm::GameplayAttribute::FindModifier(const godot::Ref<sm::ModifierData>& mod)
{
	std::vector<GameplayModifier>& mods = m_Modifiers[static_cast<size_t>(mod->GetOperationType())];

	for (auto& modifier : mods)
	{
		if (mod->GetSourceID() == modifier.sourceID)
		{
			return &modifier;
		}
	}

	return nullptr;
}

sm::GameplayModifier* sm::GameplayAttribute::FindModifier(const ModifierHandle& handle)
{
	std::vector<GameplayModifier>& mods = m_Modifiers[static_cast<size_t>(handle.op)];
	return &mods[handle.index];

	/*for (auto& modifier : *mods)
	{
		if (handle.id == modifier->ID)
		{
			return modifier.get();
		}
	}

	return nullptr;*/
}

std::optional<size_t> sm::GameplayAttribute::FindModifierIndex(const godot::Ref<sm::ModifierData>& mod) const
{
	const std::vector<GameplayModifier>& mods = m_Modifiers[static_cast<int>(mod->GetOperationType())];

	for (size_t i = 0; i < mods.size(); ++i)
	{
		if (mod->GetSourceID() == (mods)[i].sourceID)
		{
			return i;
		}
	}

	return std::nullopt;
}

sm::ModifierHandle& sm::GameplayAttribute::AddModifier(const godot::Ref<sm::ModifierData>& mod)
{
	std::vector<GameplayModifier>& mods = m_Modifiers[static_cast<int>(mod->GetOperationType())];

	ModifierID id = m_ModifiersUIDs.GenerateUID();
	ModifierOperationType type = static_cast<ModifierOperationType>(mod->GetOperationType());

	ModifierHandle handle{ id, m_ID, type, mods.size() - 1 };

	auto& modifier = mods.emplace_back(GameplayModifier{
		id,
		type,
		mod->GetValue(),
		mod->GetSourceID(),
		handle
		}
	);

	m_dirty = true;

	return modifier.handle;
}

void sm::GameplayAttribute::RemoveModifier(const godot::Ref<sm::ModifierData>& mod)
{
	std::optional<size_t> modIndex = FindModifierIndex(mod);

	std::vector<GameplayModifier>* mods = &m_Modifiers[static_cast<int>(mod->GetOperationType())];

	if (modIndex)
	{
		mods->erase(mods->begin() + *modIndex);
		m_dirty = true;
	}
}

void sm::GameplayAttribute::RemoveModifier(ModifierHandle& handle)
{
	auto& vec = m_Modifiers[static_cast<size_t>(handle.op)];

	vec[handle.index] = std::move(vec.back());
	vec[handle.index].handle.index = handle.index;

	vec.pop_back();
	m_dirty = true;
}

void sm::GameplayAttribute::AddBaseModifier(const godot::Ref<sm::ModifierData>& mod)
{
	switch (mod->GetOperationType())
	{
	case ModifierData::OperationType::Override:
		m_BaseValue = std::clamp(mod->GetValue(), m_MinValue, m_MaxValue);
		break;
	case ModifierData::OperationType::Add:
		m_BaseValue += mod->GetValue();
		break;

	case ModifierData::OperationType::Multiply:
		m_BaseValue *= mod->GetValue();
		break;

	case ModifierData::OperationType::PercentAdd:
		m_BaseValue += m_BaseValue * mod->GetValue() * 0.01f;
		break;

	default:
		break;
	}

	m_dirty = true;
}

void sm::GameplayAttribute::ClearModifiers()
{
	for (auto& arr : m_Modifiers)
	{
		arr.clear();
	}
}

void sm::GameplayAttribute::Reset()
{
	m_CurrentValue = std::clamp(m_BaseValue, m_MinValue, m_MaxValue);
	m_dirty = false;
}