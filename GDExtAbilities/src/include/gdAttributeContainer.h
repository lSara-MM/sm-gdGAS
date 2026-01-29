#pragma once
#include "GameplayAbilitySystemNode.h"
#include "GameplayAttribute.h"
#include "gdAttributeData.h"
#include "gdModifierData.h"
#include "Types.h"

#include <godot_cpp/classes/ref.hpp>
#include <memory>
#include <unordered_map>

namespace sm
{
	class Attribute;
	class AttributeSetData;
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

		godot::Ref<Attribute> GetAttribute(AttributeID id);

		godot::Ref<AttributeSetData> GetAttributeSet();
		void SetAttributeSet(const godot::Ref<AttributeSetData>& attr);

		void AddModifier(AttributeID id, const godot::Ref<ModifierData>& mod);
		void RemoveModifier(AttributeID id, const godot::Ref<ModifierData>& mod);

		// Signals
		void _OnAttributeModified(AttributeContainer& attributeContainer, AttributeID attrID, float oldValue, float newValue);

		void _OnModifierAdded(AttributeContainer attributeContainer, AttributeID attrID, godot::Ref<ModifierData> mod);

		void _OnModifierRemoved(AttributeContainer attributeContainer, AttributeID attrID, godot::Ref<ModifierData> mod);

#pragma endregion 

		void AddAttribute(godot::StringName id, float baseValue);

		void ModifyAttribute(AttributeID id, float newValue);

	private:
		AttributeContainer();
		~AttributeContainer();

	private:

		godot::Ref<AttributeSetData> m_gdAttributeSetData;
		std::unique_ptr<GameplayAttributeSet> m_AttributeSetPtr;

		std::unordered_map<godot::StringName, GameplayAttribute*> m_AttributesByName;
	};
}