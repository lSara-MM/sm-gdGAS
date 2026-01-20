#pragma once
#include "GameplayAttribute.h"
#include "gdAttribute.h"

#include <vector>

namespace sm
{
	class AttributeSet : public godot::Resource
	{
		GDCLASS(AttributeSet, godot::Resource)

	protected:
		static void _bind_methods();

	public:

#pragma region Godot public 

		godot::TypedArray<sm::Attribute> GetAttributesSet();
		void SetAttributesSet(const godot::TypedArray<sm::Attribute>& attr);

		void AddAttribute(float baseValue, godot::StringName name);
		void AddAttribute(const godot::Ref<sm::Attribute>& attr);
		
		uint32 GetAttributeID(godot::StringName name) const;

		// TODO: myb useless?
		godot::Ref<sm::Attribute> GetAttributeResource(godot::StringName name);

#pragma endregion Godot public 

		std::vector<sm::Attribute> ToAttributeVector(); // TODO: mb delete
		std::vector<godot::Ref<sm::Attribute>> ToRefAttributeVector();
		std::vector<godot::Ref<sm::Attribute>> SortByName();

		godot::TypedArray<sm::Attribute> ValidateSetData(const godot::TypedArray<sm::Attribute>& attr, bool getNull = false);

	private:
		AttributeSet() = default;
		~AttributeSet() = default;

		void _OnAttributeSetName(godot::StringName newName);

	private:
		godot::TypedArray<sm::Attribute> m_gdAttributes;
		std::unordered_map<godot::StringName, uint32> m_NamesToID;

#ifdef TOOLS_DEBUG_VS
		std::vector<std::string> m_DebugNames;
#endif //  TOOLS_DEBUG_VS
	};
}