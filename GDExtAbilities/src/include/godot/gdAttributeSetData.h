#pragma once
#include "core/GameplayAttribute.h"
#include "godot/gdAttributeData.h"
#include "godot/gdGameplayAbilitySystemResource.h"

#include <vector>

namespace sm
{
	class AttributeSetData : public GameplayAbilitySystemResource
	{
		GDCLASS(AttributeSetData, GameplayAbilitySystemResource)

	protected:
		static void _bind_methods();

	public:

#pragma region Godot public 

		godot::TypedArray<AttributeData> GetAttributesSet() const { return m_gdAttributes; };
		void SetAttributesSet(const godot::TypedArray<AttributeData>& attr);

		void AddAttribute(float baseValue, const godot::StringName& name);
		void AddAttribute(const godot::Ref<AttributeData>& attr);
		
		//uint32 GetAttributeID(godot::StringName name) const;

		// TODO: myb useless?
		godot::Ref<AttributeData> GetAttributeResource(const godot::StringName& name) const;

#pragma endregion Godot public 

		std::vector<AttributeData> ToAttributeVector(); // TODO: mb delete
		std::vector<godot::Ref<AttributeData>> ToRefAttributeVector();
		std::vector<godot::Ref<AttributeData>> SortByName();

		const godot::TypedArray<AttributeData>& ValidateSetData(const godot::TypedArray<AttributeData>& attr, bool getNull = false);

	private:
		AttributeSetData() = default;
		~AttributeSetData() = default;

		void _OnAttributeSetName(const godot::StringName& newName);

	private:
		godot::TypedArray<AttributeData> m_gdAttributes;

#ifdef TOOLS_DEBUG_VS
		std::vector<std::string> m_DebugNames;
#endif //  TOOLS_DEBUG_VS
	};
}