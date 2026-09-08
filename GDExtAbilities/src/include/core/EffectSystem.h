#pragma once
#include "core/GameplayEffect.h"
#include "core/GameplaySystem.h"

#include <unordered_map>

namespace sm
{
	class GAS_Entity;
	class GAS_World;

	class EffectSystem : public GameplaySystem
	{

	public:
		explicit EffectSystem(GAS_World* w) { _world = w; }
		~EffectSystem() = default;
		void Update(float dt) override;

		GameplayEffect* FindEffect(EffectID effectID);
		GameplayEffect* FindEffect(EffectInstanceID effectID);

		EffectInstanceID AddActiveEffect(GameplayEffect& effect);
		void RemoveEffect(GameplayEffect* effect);
		void RemoveEffect(EffectInstanceID effectID, GAS_Entity* entity);

		void RemoveEffectModifiers(GAS_Entity* entity, GameplayEffect* effect);

		void ClearEffects(GAS_Entity* entity);

	public:
		DumbUID m_EffectsID;

	private:
		std::vector<GameplayEffect> m_ActiveEffects;
		std::unordered_map<EffectInstanceID, size_t> m_EffectsIndex;
	};
}
