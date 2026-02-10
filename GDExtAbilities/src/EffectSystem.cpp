#include "EffectSystem.h"

void sm::EffectSystem::Update(float dt)
{
	for (auto& effect : m_ActiveEffects)
	{
		effect->Tick(dt);
	}
}

sm::GameplayEffect* sm::EffectSystem::FindEffect(EffectID effectID)
{
	auto it = std::find_if(m_ActiveEffects.begin(), m_ActiveEffects.end(),
		[&](const EffectPtr& e)
		{
			return e->GetUID() == effectID;
		}
	);

	return (it != m_ActiveEffects.end()) ? it->get() : nullptr;
}

void sm::EffectSystem::AddEffect(godot::Ref<EffectData> gdEffect)
{
	m_ActiveEffects.emplace_back(std::make_unique<GameplayEffect>(
		gdEffect->GetName(),
		static_cast<GameplayEffect::Type>(gdEffect->GetEffectType())
	));


	godot::TypedArray<ModifierData> modifiers = gdEffect->GetModifiers();
	/*for (size_t i = 0; i < modifiers.size(); i++)
	{
		attr->AddModifier(modifiers[i]);
	}*/
}

void sm::EffectSystem::RemoveEffect(GameplayAttribute& attr, godot::Ref<EffectData> gdEffect)
{
	GameplayEffect effect = { gdEffect->GetName(),  static_cast<GameplayEffect::Type>(gdEffect->GetEffectType()) };

	m_ActiveEffects.emplace_back(std::make_unique<GameplayEffect>(
		gdEffect->GetName(),
		static_cast<GameplayEffect::Type>(gdEffect->GetEffectType())
	));

	attr.ClearModifiers();
}

void sm::EffectSystem::RemoveEffect(GameplayAttribute& attr, EffectID gdEffectID)
{
	m_ActiveEffects.erase(std::remove_if(m_ActiveEffects.begin(), m_ActiveEffects.end(),
		[&](const std::unique_ptr<sm::GameplayEffect>& effect)
		{
			return effect->GetUID() == gdEffectID;
		}),
		m_ActiveEffects.end()
	);

	attr.ClearModifiers();
}
