#pragma once
#include "GameplayAttribute.h"

#include <vector>

namespace sm
{
	class GameplayAttributeSet
	{
	public:
		GameplayAttributeSet();
		~GameplayAttributeSet();

		void AddAttribute(uint32 id, float base, float min = 0.0f, float max = FLT_MAX);

	private:
		std::vector<GameplayAttribute> m_Attributes;
	};

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
		godot::Ref<sm::Attribute> GetAttribute(godot::StringName name);

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

#ifdef TOOLS_DEBUG_VS
		std::vector<std::string> m_DebugNames;
#endif //  TOOLS_DEBUG_VS
	};
}