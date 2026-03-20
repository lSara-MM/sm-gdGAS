#pragma once
#include "core/GameplayEffect.h"
#include "core/GameplaySystem.h"

namespace sm
{
	class GAS_Entity;

	class EffectSystem : public GameplaySystem
	{

	public:
		EffectSystem() = default;
		~EffectSystem() = default;
		void Update(float dt) override;

		GameplayEffect* FindEffect(EffectID effectID);

		void AddActiveEffect(GameplayEffect& effect);
		//void RemoveEffect(EntityID id, const godot::Ref<EffectData> gdEffect);

		//void RemoveEffect(EffectID effectID);
		void RemoveEffect(GameplayEffect& effect);

		void RemoveEffectModifiers(GAS_Entity* entity, std::vector<GameplayEffect>::iterator& itr);

		void RemoveEffectModifiers(GAS_Entity* entity, GameplayEffect& effect);

	public:
		DumbUID m_EffectsID;

	private:
		std::vector<GameplayEffect> m_ActiveEffects;
	};
}