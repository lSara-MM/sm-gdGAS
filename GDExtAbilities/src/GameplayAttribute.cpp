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
	if (!m_Modifiers[static_cast<size_t>(GameplayModifier::OperationType::Override)].empty())
	{
		// If there are more than one, apply last
		auto& modifier = m_Modifiers[static_cast<size_t>(GameplayModifier::OperationType::Override)];

		m_CurrentValue = modifier[modifier.size() - 1]->value;
		m_CurrentValue = std::clamp(m_CurrentValue, m_MinValue, m_MaxValue);

		return;
	}

	float sum = 0;
	for (auto& modifier : m_Modifiers[static_cast<size_t>(GameplayModifier::OperationType::Add)])
	{
		sum += modifier->value;
	}

	float mult = 1;
	for (auto& modifier : m_Modifiers[static_cast<size_t>(GameplayModifier::OperationType::Multiply)])
	{
		mult *= modifier->value;
	}

	float perAdd = 0;
	for (auto& modifier : m_Modifiers[static_cast<size_t>(GameplayModifier::OperationType::PercentAdd)])
	{
		perAdd += modifier->value;
	}

	float perStack = 1;
	for (auto& modifier : m_Modifiers[static_cast<size_t>(GameplayModifier::OperationType::PercentStack)])
	{
		perStack *= 1.0f + modifier->value * 0.01f;
	}

	//m_CurrentValue += sum;
	//m_CurrentValue *= mult;
	//m_CurrentValue += m_BaseValue * perAdd * 0.01f;
	//m_CurrentValue *= perStack;
	m_CurrentValue = ((m_BaseValue + sum) * mult + m_BaseValue * perAdd * 0.01f) * perStack;

	m_CurrentValue = std::clamp(m_CurrentValue, m_MinValue, m_MaxValue);
}

void sm::GameplayAttribute::SetBase(float newValue)
{
	m_BaseValue = std::clamp(newValue, m_MinValue, m_MaxValue);
}

sm::GameplayModifier* sm::GameplayAttribute::FindModifier(const godot::Ref<sm::ModifierData>& mod)
{
	std::vector<std::unique_ptr<sm::GameplayModifier>>* mods = &m_Modifiers[static_cast<size_t>(mod->GetOperationType())];

	for (auto& modifier : *mods)
	{
		if (mod->GetSourceID() == modifier->sourceID)
		{
			return modifier.get();
		}
	}

	return nullptr;
}

std::optional<size_t> sm::GameplayAttribute::FindModifierIndex(const godot::Ref<sm::ModifierData>& mod) const
{
	const std::vector<ModifierPtr>* mods = &m_Modifiers[static_cast<int>(mod->GetOperationType())];

	for (size_t i = 0; i < mods->size(); ++i)
	{
		if (mod->GetSourceID() == (*mods)[i]->sourceID)
		{
			return i;
		}
	}

	return std::nullopt;
}

size_t sm::GameplayAttribute::AddModifier(const godot::Ref<sm::ModifierData>& mod)
{
	std::vector<ModifierPtr>* mods = &m_Modifiers[static_cast<int>(mod->GetOperationType())];

	mods->emplace_back(std::make_unique<GameplayModifier>(
		m_ModifiersUID.GenerateUID(),
		static_cast<GameplayModifier::OperationType>(mod->GetOperationType()),
		mod->GetTargetID(),
		mod->GetSourceID(),
		mod->GetValue()
	));

	m_dirty = true;

	return mods->size() - 1;
}

void sm::GameplayAttribute::RemoveModifier(const godot::Ref<sm::ModifierData>& mod)
{
	std::optional<size_t> modIndex = FindModifierIndex(mod);

	std::vector<ModifierPtr>* mods = &m_Modifiers[static_cast<int>(mod->GetOperationType())];

	if (modIndex.has_value())
	{
		mods->erase(mods->begin() + modIndex.value());
		m_dirty = true;
	}
}

void sm::GameplayAttribute::Reset()
{
	m_CurrentValue = std::clamp(m_BaseValue, m_MinValue, m_MaxValue);
	m_dirty = false;
}