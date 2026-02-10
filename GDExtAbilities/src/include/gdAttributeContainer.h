#pragma once
#include "EffectSystem.h"
#include "GameplayAttribute.h"
#include "gdAttributeData.h"
#include "gdAttributeSetData.h"
#include "gdEffectData.h"
#include "gdGameplayAbilitySystemNode.h"
#include "gdModifierData.h"
#include "Types.h"

#include <godot_cpp/classes/ref.hpp>
#include <unordered_map>

namespace sm
{
	class Attribute;
	class GameplayAttributeSet;

	struct GameplayModifier;
}

namespace sm
{
	class AttributeContainer : public GameplayAbilitySystem
	{
		GDCLASS(AttributeContainer, GameplayAbilitySystem)

	protected:
		static void _bind_methods();

	public:

		// On any notification. (similar to OnCollision from Unity)
		void _notification(int notification);

#pragma region Godot public 

		godot::Ref<Attribute> GetAttribute(AttributeID id) const;

		godot::Ref<AttributeSetData> GetAttributeSet() const;
		void SetAttributeSet(const godot::Ref<AttributeSetData>& attr);

		// TODO: Change to effect? dont allow add/remove direct modifier
		void AddModifier(AttributeID id, godot::Ref<ModifierData> mod);
		void RemoveModifier(AttributeID id, godot::Ref<ModifierData> mod);

		void AddEffect(AttributeID id, godot::Ref<EffectData> effect);
		void RemoveEffect(AttributeID id, godot::Ref<EffectData> effect);
		void RemoveEffect(AttributeID id, EffectID effectID);

		// TODO: Allow direct base modification?

#pragma endregion 

		void AddAttribute(godot::StringName id, float baseValue);

		void ModifyAttribute(AttributeID id, float newValue);

	private:
		AttributeContainer();
		~AttributeContainer();

	private:

		std::unique_ptr<GameplayAttributeSet> m_AttributeSetPtr;
		std::shared_ptr<EffectSystem> m_EffectSystemPtr;
		std::unordered_map<godot::StringName, GameplayAttribute*> m_AttributesByName;
		
		godot::Ref<AttributeSetData> m_gdAttributeSetData;
	};
}