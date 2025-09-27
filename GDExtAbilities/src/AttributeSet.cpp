#include "AttributeSet.h"

sm::AttributeSet::AttributeSet()
{}

sm::AttributeSet::~AttributeSet()
{}

void sm::AttributeSet::_bind_methods()
{

}

void sm::AttributeSetPreset::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_ability_properties"), &GetAbilityPropertiesEd);
	godot::ClassDB::bind_method(godot::D_METHOD("set_ability_properties", "arr"), &SetAbilityPropertiesEd);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::ARRAY,
		"Properties",
		godot::PROPERTY_HINT_TYPE_STRING,
		godot::String::num(godot::Variant::OBJECT) + "/" + godot::String::num(godot::PROPERTY_HINT_RESOURCE_TYPE) + ":Attribute"),
		"set_ability_properties", "get_ability_properties"
	);

	godot::ClassDB::bind_method(godot::D_METHOD("add_property", "property"), &AddProperty);
}

sm::AttributeSetPreset::AttributeSetPreset()
{}

sm::AttributeSetPreset::~AttributeSetPreset()
{}

void sm::AttributeSetPreset::SetAbilityPropertiesEd(const godot::TypedArray<sm::Attribute> arr)
{
	m_AbilityProperties.clear();
	m_AbilityPropertiesEd.clear();

	for (int i = 0; i < arr.size(); i++)
	{
		godot::Ref<Attribute> property = arr[i];
		if (property.is_valid())
		{
			godot::Ref<Attribute> added = AddProperty(property);
			if (added.is_valid())
			{
				m_AbilityPropertiesEd.append(added);
			}
		}
		else
		{
			m_AbilityPropertiesEd.append(property);
		}
	}
}

godot::Ref<sm::Attribute> sm::AttributeSetPreset::AddProperty(godot::Ref<sm::Attribute> property)
{
	using TAbilitiesMap = std::unordered_map<std::type_index, godot::Ref<Attribute>>;

	std::pair<TAbilitiesMap::iterator, bool> result = m_AbilityProperties.emplace(std::type_index(typeid(*property.ptr())), property);

	if (!result.second)
	{
		WARN_PRINT_ED("sm: Property already existing");
		return nullptr;
	}

	return result.first->second;
}