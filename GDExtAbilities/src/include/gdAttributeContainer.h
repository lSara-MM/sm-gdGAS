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
	class AttributeSetData;
	class GameplayAttributeSet;

	struct GameplayModifier;
}

namespace sm
{
	class AttributeContainer : public sm::GameplayAbilitySystem
	{
		GDCLASS(AttributeContainer, sm::GameplayAbilitySystem)

	protected:
		static void _bind_methods();

	public:

		// On any notification. (similar to OnCollision from Unity)
		void _notification(int notification);

#pragma region Godot public 

		godot::Ref<sm::AttributeSetData> GetAttributeSet();
		void SetAttributeSet(const godot::Ref<sm::AttributeSetData>& attr);

		void AddModifier(AttributeID id, const godot::Ref<sm::ModifierData>& mod);
		void RemoveModifier(AttributeID id, const godot::Ref<sm::ModifierData>& mod);

		// Signals
		void _OnAttributeModified(sm::AttributeContainer& attributeContainer, AttributeID attrID, float oldValue, float newValue);

		void _OnModifierAdded(sm::AttributeContainer attributeContainer, AttributeID attrID, godot::Ref<sm::ModifierData> mod);

		void _OnModifierRemoved(sm::AttributeContainer attributeContainer, AttributeID attrID, godot::Ref<sm::ModifierData> mod);

#pragma endregion 

		void ModifyAttribute(AttributeID id, float newValue);

	private:
		AttributeContainer();
		~AttributeContainer();

	private:

		godot::Ref<sm::AttributeSetData> m_gdAttributeSet;
		std::unique_ptr<sm::GameplayAttributeSet> m_AttributeSetPtr;

		std::unordered_map<godot::StringName, sm::GameplayAttribute*> m_AttributesByName;
	};
}