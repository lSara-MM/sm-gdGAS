#pragma once
#include "Attribute.h"
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <typeindex>

namespace sm
{
	class AttributeSet : public godot::Resource
	{
		GDCLASS(AttributeSet, godot::Resource)

	protected:
		static void _bind_methods();

	public:
		AttributeSet();
		~AttributeSet();
	};

	class AttributeSetPreset : public AttributeSet
	{
		GDCLASS(AttributeSetPreset, AttributeSet)
		
	protected:
		static void _bind_methods();

	public:
		AttributeSetPreset();
		~AttributeSetPreset();

		godot::TypedArray<sm::Attribute> GetAbilityPropertiesEd() { return m_AbilityPropertiesEd; }
		void SetAbilityPropertiesEd(const godot::TypedArray<sm::Attribute> arr);

		godot::Ref<sm::Attribute> AddProperty(godot::Ref<sm::Attribute> property);

	public:
		godot::TypedArray<sm::Attribute> m_AbilityPropertiesEd;
		std::unordered_map<std::type_index, godot::Ref<sm::Attribute>> m_AbilityProperties;
	};
}