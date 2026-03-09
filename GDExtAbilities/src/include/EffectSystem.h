#pragma once
#include "GameplayEffect.h"
#include "GameplaySystem.h"
#include "gdEffectData.h"

#include <godot_cpp/classes/node.hpp>

namespace sm
{
	class GAS_Entity;

	class EffectSystem : public GameplaySystem
	{

	public:
		EffectSystem()/* = default*/;
		~EffectSystem()/* = default*/;
		void Update(float dt) override;

		GameplayEffect* FindEffect(EffectID effectID);

		void AddActiveEffect(GameplayEffect& effect);
		void RemoveEffect(EntityID id, const godot::Ref<EffectData> gdEffect);

		void RemoveEffect(EffectID effectID);
		void RemoveEffect(GameplayEffect& effect);

		void RemoveEffectModifiers(EntityID id, std::vector<GameplayEffect>::iterator& itr);

		void RemoveEffectModifiers(GAS_Entity* entity, GameplayEffect& effect);

		template <typename T>
		T* GetChildNodeOfType(godot::Node* target)
		{
			for (int i = 0; i < target->get_child_count(); ++i)
			{
				auto* node = godot::Object::cast_to<T>(
					target->get_child(i)
				);

				if (node)
				{
					return node;
				}
			}

			return nullptr;
		}

	public:
		DumbUID m_EffectsID;

	private:
		std::vector<GameplayEffect> m_ActiveEffects;
	};
}