#include "GameplayAttribute.h"

#include "gdModifierData.h"

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

sm::GameplayModifier* sm::GameplayAttribute::FindModifier(const godot::Ref<sm::ModifierData>& mod)
{
	std::vector<std::unique_ptr<sm::GameplayModifier>>* mods = &m_Modifiers[static_cast<int>(mod->GetOperationType())];

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
	const std::vector<std::unique_ptr<sm::GameplayModifier>>* mods = &m_Modifiers[static_cast<int>(mod->GetOperationType())];

	for (size_t i = 0; i < mods->size(); ++i)
	{
		if (mod->GetSourceID() == (*mods)[i]->sourceID)
		{
			return i;
		}
	}

	return std::nullopt;
}

void sm::GameplayAttribute::AddModifier(const godot::Ref<sm::ModifierData>& mod)
{
	std::vector<std::unique_ptr<sm::GameplayModifier>>* mods = &m_Modifiers[static_cast<int>(mod->GetOperationType())];

	mods->emplace_back(std::make_unique<sm::GameplayModifier>(
		m_ModifiersUID.GenerateUID(),
		static_cast<GameplayModifier::OperationType>(mod->GetOperationType()),
		mod->GetTargetID(),
		mod->GetSourceID(),
		mod->GetValue()
	));

	m_dirty = true;
}

void sm::GameplayAttribute::RemoveModifier(const godot::Ref<sm::ModifierData>& mod)
{
	std::optional<size_t> modIndex = FindModifierIndex(mod);

	std::vector<std::unique_ptr<sm::GameplayModifier>>* mods = &m_Modifiers[static_cast<int>(mod->GetOperationType())];

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