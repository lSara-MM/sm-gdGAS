#pragma once
#include "core/GameplayAttribute.h"
#include "godot/gdAttributeSetData.h"
#include "godot/gdGameplayAbilitySystemNode.h"
#include "godot/gdModifierData.h"
#include "internal/Types.h"

#include <godot_cpp/classes/ref.hpp>
#include <unordered_map>

namespace sm
{
	class GameplayAttributeSet;
}

namespace sm
{
	class AttributeContainer : public GameplayAbilitySystem
	{
		GDCLASS(AttributeContainer, GameplayAbilitySystem)

	protected:
		static void _bind_methods();

	public:
#pragma region Godot public 
		// TODO: unused?
		//godot::Ref<Attribute> GetAttribute(AttributeID id) const;
		godot::Node* GetOwnerNode() const { return m_OwnerNode; }
		godot::NodePath GetOwnerNodePath() const { return m_OwnerNodePath; }
		void SetOwnerNodePath(godot::NodePath path = "");

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

#pragma endregion

		GameplayAttribute* FindAttribute(AttributeID id) const;
		ModifierID AddModifier(GameplayAttribute* attr, godot::Ref<ModifierData> mod);
		void AddAttribute(godot::StringName id, float baseValue);
		void ModifyAttribute(AttributeID id, float newValue);

	private:
		AttributeContainer();
		~AttributeContainer();

		// When node container is loaded, get editor changes and apply them to the C++ AttributeData Set
		void OnReady() override;

	private:
		godot::Node* m_OwnerNode = nullptr;
		godot::NodePath m_OwnerNodePath = "";
		std::unique_ptr<GameplayAttributeSet> m_AttributeSetPtr;
		std::unordered_map<godot::StringName, GameplayAttribute*> m_AttributesByName;

		godot::Ref<AttributeSetData> m_gdAttributeSetData;
	};
}