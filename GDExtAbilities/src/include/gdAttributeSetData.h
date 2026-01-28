#pragma once
#include "GameplayAttribute.h"
#include "gdAttributeData.h"

#include <vector>

namespace sm
{
	class AttributeSetData : public godot::Resource
	{
		GDCLASS(AttributeSetData, godot::Resource)

	protected:
		static void _bind_methods();

	public:

#pragma region Godot public 

		godot::TypedArray<sm::AttributeData> GetAttributesSet();
		void SetAttributesSet(const godot::TypedArray<sm::AttributeData>& attr);

		void AddAttribute(float baseValue, godot::StringName name);
		void AddAttribute(const godot::Ref<sm::AttributeData>& attr);
		
		//uint32 GetAttributeID(godot::StringName name) const;

		// TODO: myb useless?
		godot::Ref<sm::AttributeData> GetAttributeResource(godot::StringName name);

#pragma endregion Godot public 

		std::vector<sm::AttributeData> ToAttributeVector(); // TODO: mb delete
		std::vector<godot::Ref<sm::AttributeData>> ToRefAttributeVector();
		std::vector<godot::Ref<sm::AttributeData>> SortByName();

		godot::TypedArray<sm::AttributeData> ValidateSetData(const godot::TypedArray<sm::AttributeData>& attr, bool getNull = false);

	private:
		AttributeSetData() = default;
		~AttributeSetData() = default;

		void _OnAttributeSetName(godot::StringName newName);

	private:
		godot::TypedArray<sm::AttributeData> m_gdAttributes;

#ifdef TOOLS_DEBUG_VS
		std::vector<std::string> m_DebugNames;
#endif //  TOOLS_DEBUG_VS
	};
}