#pragma once
#include "GameplayAttribute.h"
#include "GameplayEffect.h"
#include "GameplaySystem.h"
#include "gdEffectData.h"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>

namespace sm
{
	class EffectSystem : public GameplaySystem
	{
	private:
		using EffectPtr = std::unique_ptr<GameplayEffect>;

	public:
		EffectSystem() = default;
		void Update(float dt) override;

		GameplayEffect* FindEffect(EffectID effectID);

		void AddEffect(godot::Ref<EffectData> gdEffect, godot::Node* target);

		void RemoveEffect(GameplayAttribute& attr, godot::Ref<EffectData> gdEffect);

		void RemoveEffect(GameplayAttribute& attr, EffectID gdEffectID);

		template <typename T>
		T* GetNodeOfType(godot::Node* target)
		{
			godot::Node* parent = target->get_parent();
			for (int i = 0; i < parent->get_child_count(); ++i)
			{
				auto* node = godot::Object::cast_to<T>(
					parent->get_child(i)
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