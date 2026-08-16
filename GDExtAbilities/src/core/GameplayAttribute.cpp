#include "core/GameplayAttribute.h"

#include "godot/gdModifierData.h"

#include <algorithm>

float sm::GameplayAttribute::Calculate()
{
	if (!m_dirty)
	{
		return m_CurrentValue;
	}

	float current = 0;

	// Ignore all modifiers if there is an Override
	if (!m_Modifiers[static_cast<size_t>(ModifierOperationType::Override)].empty())
	{
		// If there are more than one, apply last
		auto& modifier = m_Modifiers[static_cast<size_t>(ModifierOperationType::Override)];

		current = modifier[modifier.size() - 1].value;
		current = std::clamp(current, m_MinValue, m_MaxValue);

		m_dirty = false;
		return current;
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
	current = ((m_BaseValue + sum) * mult + m_BaseValue * perAdd * 0.01f) * perStack;
	current = std::clamp(current, m_MinValue, m_MaxValue);

	m_dirty = false;

	return current;
}

void sm::GameplayAttribute::SetBase(float newValue)
{
	if (m_BaseValue == newValue)
	{
		return;
	}

	m_BaseValue = std::clamp(newValue, m_MinValue, m_MaxValue);
	m_dirty = true;
	GetCurrent();
}

float sm::GameplayAttribute::GetCurrent()
{
	if (m_dirty)
	{
		float oldVal = m_CurrentValue;
		float rawValue = Calculate();
		m_CurrentValue = m_PreAttrChange ? m_PreAttrChange(rawValue) : rawValue;

		if (oldVal != m_CurrentValue)
		{
			m_AttrChange(m_CurrentValue, oldVal);
		}
	}

	return m_CurrentValue;
}

void sm::GameplayAttribute::SetPreAttributeChange(std::function<float(float)> delegate)
{
	m_PreAttrChange = delegate;
}

void sm::GameplayAttribute::SetAttributeChanged(std::function<void(float, float)> delegate)
{
	m_AttrChange = delegate;
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
	float rawValue = m_BaseValue;

	switch (mod->GetOperationType())
	{
	case ModifierData::OperationType::Override:
		rawValue = mod->GetValue();
		break;
	case ModifierData::OperationType::Add:
		rawValue = m_BaseValue + mod->GetValue();
		break;

	case ModifierData::OperationType::Multiply:
		rawValue = m_BaseValue * mod->GetValue();
		break;

	case ModifierData::OperationType::PercentAdd:
		rawValue = m_BaseValue + m_BaseValue * mod->GetValue() * 0.01f;
		break;

	default:
		break;
	}

	if (m_PreAttrChange)
	{
		SetBase(m_PreAttrChange(rawValue));
	}
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