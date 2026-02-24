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
		// TODO: unused?
		godot::Ref<Attribute> GetAttribute(AttributeID id) const;

		godot::Ref<AttributeSetData> GetAttributeSet() const;
		void SetAttributeSet(const godot::Ref<AttributeSetData>& attr);

		float GetAttributeBaseValue(AttributeID id);
		float GetAttributeCurrentValue(AttributeID id);
		bool IsAttributeMin(AttributeID id);
		bool IsAttributeMax(AttributeID id);
		bool IsAttributeDirty(AttributeID id);

		ModifierID AddModifier(AttributeID id, godot::Ref<ModifierData> mod);
		void AddBaseModifier(AttributeID id, godot::Ref<ModifierData> mod);
		void RemoveModifier(AttributeID id, godot::Ref<ModifierData> mod);

		/*void AddEffect(EntityID id, godot::Ref<EffectData> effect);
		void RemoveEffect(EntityID id, godot::Ref<EffectData> effect);
		void RemoveEffect(EntityID id, EffectID effectID);*/

		// TODO: Allow direct base modification?

#pragma endregion

		GameplayAttribute* FindAttribute(AttributeID id) const;

		ModifierID AddModifier(GameplayAttribute* attr, godot::Ref<ModifierData> mod);

		void AddAttribute(godot::StringName id, float baseValue);

		void ModifyAttribute(AttributeID id, float newValue);

	private:
		AttributeContainer();
		~AttributeContainer();

		void OnEnterTree();
		void OnExitTree();

		// When node container is loaded, get editor changes and apply them to the C++ AttributeData Set
		void OnReady();

	private:

		std::unique_ptr<GameplayAttributeSet> m_AttributeSetPtr;
		//std::shared_ptr<EffectSystem> m_EffectSystemPtr;
		std::unordered_map<godot::StringName, GameplayAttribute*> m_AttributesByName;
		
		godot::Ref<AttributeSetData> m_gdAttributeSetData;
	};
}