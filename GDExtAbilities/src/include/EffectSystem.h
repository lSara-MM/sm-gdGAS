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
	private:
		using EffectPtr = std::unique_ptr<GameplayEffect>;

	public:
		EffectSystem()/* = default*/;
		~EffectSystem()/* = default*/;
		void Update(float dt) override;

		GameplayEffect* FindEffect(EffectID effectID);

		void AddActiveEffect(EffectPtr& effect);
		void RemoveEffect(EntityID id, const godot::Ref<EffectData> gdEffect);

		void RemoveEffect(EffectID effectID);
		void RemoveEffect(EffectPtr& effect);

		void RemoveEffectModifiers(EntityID id, std::vector<EffectSystem::EffectPtr>::iterator& itr);

		void RemoveEffectModifiers(GAS_Entity* entity, EffectPtr& effect);

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

	private:
		//std::vector<EffectPtr> m_PermanentEffects;
		//std::vector<EffectPtr> m_TemporaryEffects;
		//std::vector<EffectPtr> m_PersistentEffects;
		std::vector<EffectPtr> m_ActiveEffects;
	};
}