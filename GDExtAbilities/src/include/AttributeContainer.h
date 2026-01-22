#pragma once
#include "GameplayAbilitySystemNode.h"
#include "GameplayAttribute.h"
#include "gdAttribute.h"
#include "gdModifier.h"
#include "Types.h"

#include <godot_cpp/classes/ref.hpp>
#include <memory>
#include <unordered_map>

namespace sm
{
	class AttributeSet;
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

		godot::Ref<sm::AttributeSet> GetAttributeSet();
		void SetAttributeSet(const godot::Ref<sm::AttributeSet>& attr);

		AttributeID GetAttributeID(godot::StringName name) const;

		void AddModifier(AttributeID id, const godot::Ref<sm::Modifier> mod);
		void RemoveModifier(AttributeID id, const godot::Ref<sm::Modifier> mod);

		// Signals
		void _OnAttributeModified(sm::AttributeContainer& attributeContainer, AttributeID attrID, float oldValue, float newValue);

		void _OnModifierAdded(sm::AttributeContainer attributeContainer, AttributeID attrID, godot::Ref<sm::Modifier> mod);

		void _OnModifierRemoved(sm::AttributeContainer attributeContainer, AttributeID attrID, godot::Ref<sm::Modifier> mod);

#pragma endregion 

		void ModifyAttribute(uint32 id, float newValue);

	private:
		AttributeContainer();
		~AttributeContainer();

		uint32 _GenerateUID() { return m_NextUID++; }

	private:
		uint32 m_NextUID = 0;

		godot::Ref<sm::AttributeSet> m_gdAttributeSet;
		std::unique_ptr<sm::GameplayAttributeSet> m_AttributeSetPtr;

		std::unordered_map<godot::StringName, sm::GameplayAttribute> m_AttributesByName;
	};
}