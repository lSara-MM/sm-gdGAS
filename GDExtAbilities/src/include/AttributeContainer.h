#pragma once
#include "GameplayAbilitySystemNode.h"
#include "gdAttribute.h"
#include "Types.h"

#include <godot_cpp/classes/ref.hpp>
#include <memory>
#include <unordered_map>

namespace sm
{
	class AttributeSet;
	class GameplayAttributeSet;

	struct Modifier;
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

		AttributeID GetAttributeID(godot::StringName name);
		void AddModifier(AttributeID id, sm::Modifier mod);

			// Signals
		void _OnAttributeModified(sm::AttributeContainer& attributeContainer, godot::StringName attrName, float oldValue, float newValue);
		
		//void _OnModifierAdded(sm::AttributeContainer attributeContainer, godot::StringName attrName, sm::Modifier mod);

#pragma endregion 
		
		//void ModifyAttribute(uint32 id, float newValue);

	private:
		AttributeContainer();
		~AttributeContainer();

		uint32 _GenerateUID() { return m_NextUID++; }

	private:
		uint32 m_NextUID = 0;
		godot::Ref<sm::AttributeSet> m_gdAttributeSet;
		std::unique_ptr<sm::GameplayAttributeSet> m_AttributeSetPtr;

		//std::unordered_map<godot::StringName, sm::GameplayAttribute> m_AttributesByName;
	};
}