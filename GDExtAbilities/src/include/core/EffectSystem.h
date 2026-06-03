#pragma once
#include "core/GameplayEffect.h"
#include "core/GameplaySystem.h"

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

		void AddActiveEffect(GameplayEffect& effect);
		//void RemoveEffect(EntityID id, const godot::Ref<EffectData> gdEffect);

		//void RemoveEffect(EffectID effectID);
		void RemoveEffect(GameplayEffect* effect, size_t index);

		void RemoveEffectModifiers(GAS_Entity* entity, GameplayEffect* effect);

	public:
		DumbUID m_EffectsID;

	private:
		std::vector<GameplayEffect> m_ActiveEffects;
	};
}
