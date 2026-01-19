#include "AttributeSet.h"

#include <algorithm>
#include <unordered_set>
#include <variant/utility_functions.hpp>

sm::GameplayAttributeSet::GameplayAttributeSet()
{}

sm::GameplayAttributeSet::~GameplayAttributeSet()
{}

void sm::GameplayAttributeSet::AddAttribute(uint32 id, float base, float min, float max)
{
	m_Attributes.emplace_back(id, base, min, max);
}

void sm::AttributeSet::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_attributes_set"), &GetAttributesSet);
	godot::ClassDB::bind_method(godot::D_METHOD("set_attributes_set", "attr"), &SetAttributesSet);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::ARRAY,
		"attributes",
		godot::PROPERTY_HINT_RESOURCE_TYPE,
		"24/17:Attribute"),
		"set_attributes_set", "get_attributes_set");

	godot::ClassDB::bind_method(godot::D_METHOD("add_attribute", "attr"), &SetAttributesSet);
}

godot::TypedArray<sm::Attribute> sm::AttributeSet::GetAttributesSet()
{
	return m_gdAttributes;
}

void sm::AttributeSet::SetAttributesSet(const godot::TypedArray<sm::Attribute>& attr)
{
	m_gdAttributes = attr;

	for (int i = 0; i < attr.size(); i++)
	{
		const godot::Ref<sm::Attribute> attribute = attr[i];

#ifdef TOOLS_DEBUG_VS
		if (attribute.is_null())
		{
			m_DebugNames.push_back("<empty>");
		}
		else
		{
			m_DebugNames.push_back(ToStdString(attribute->GetName()));
		}
#endif //  TOOLS_DEBUG_VS

		if (attribute.is_null())
		{
			continue;
		}

#ifdef DEBUG_MODE
		attribute->SubscribeSetNameEvent([this](const godot::StringName& new_name)
			{
				this->_OnAttributeSetName(new_name);
			});
#endif // DEBUG_MODE
	}
}

void sm::AttributeSet::AddAttribute(float baseValue, godot::StringName name)
{
	godot::Ref<sm::Attribute> attr;
	attr.instantiate();
	attr->SetBaseValue(baseValue);
	attr->SetName(name);
	m_gdAttributes.append(attr);

	emit_changed();
}

void sm::AttributeSet::AddAttribute(const godot::Ref<sm::Attribute>& attr)
{
	m_gdAttributes.push_back(attr);

	emit_changed();
}

godot::Ref<sm::Attribute> sm::AttributeSet::GetAttribute(godot::StringName name)
{
	godot::Ref<sm::Attribute> ref;

	for (size_t i = 0; i < m_gdAttributes.size(); i++)
	{
		ref = m_gdAttributes[i];
		if (ref->GetName() == name)
		{
			return ref;
		}
	}

	return godot::Ref<sm::Attribute>();
}

std::vector<sm::Attribute> sm::AttributeSet::ToAttributeVector()
{
	std::vector<sm::Attribute> ret;

	for (int i = 0; i < m_gdAttributes.size(); ++i)
	{
		godot::Ref<sm::Attribute> ref = m_gdAttributes[i];
		ret.emplace_back(ref->GetBaseValue());
	}

	return ret;
}

std::vector<godot::Ref<sm::Attribute>> sm::AttributeSet::ToRefAttributeVector()
{
	std::vector<godot::Ref<sm::Attribute>> ret;
	ret.reserve(m_gdAttributes.size());

	for (int i = 0; i < m_gdAttributes.size(); ++i)
	{
		ret.emplace_back(m_gdAttributes[i]);
	}

	return ret;
}

std::vector<godot::Ref<sm::Attribute>> sm::AttributeSet::SortByName()
{
	std::vector<godot::Ref<sm::Attribute>> ret = ToRefAttributeVector();

	//// TODO: delete
	//std::vector<std::string> ret2;
	//ret2.reserve(ret.size());

	//for (int i = 0; i < ret.size(); ++i)
	//{
	//	ret2.emplace_back(ToStdString(ret[i]->GetName()));
	//}
	////

	ret.erase(
		std::remove_if(ret.begin(), ret.end(),
			[](const godot::Ref<sm::Attribute>& attr)
			{
				return attr.is_null();
			}),
		ret.end()
	);


	std::sort(ret.begin(), ret.end(),
		[](const godot::Ref<sm::Attribute>& a, const godot::Ref<sm::Attribute>& b)
		{
			return a->GetName() < b->GetName();
		});

	//// TODO: delete
	//std::vector<std::string> ret3;
	//ret3.reserve(ret.size());

	//for (int i = 0; i < ret.size(); ++i)
	//{
	//	ret3.emplace_back(ToStdString(ret[i]->GetName()));
	////}

	return ret;
}

godot::TypedArray<sm::Attribute> sm::AttributeSet::ValidateSetData(const godot::TypedArray<sm::Attribute>& attr, bool getNull)
{
	godot::TypedArray<sm::Attribute> validatedSet;
	std::unordered_set<godot::StringName> seen;

	for (int i = 0; i < attr.size(); i++)
	{
		const godot::Ref<sm::Attribute> itemToCompare = attr[i];

		if (itemToCompare.is_null())
		{
			if (getNull)
			{
				validatedSet.push_back(itemToCompare);
			}
			continue;
		}

		const godot::StringName name = itemToCompare->GetName();
		if (!name.is_empty() && seen.find(itemToCompare->GetName()) != seen.end())
		{
			continue;
		}

		seen.emplace(name);
		validatedSet.push_back(itemToCompare);
	}

	return validatedSet;
}

void sm::AttributeSet::_OnAttributeSetName(godot::StringName newName)
{
	for (int i = 0; i < m_gdAttributes.size(); i++)
	{
		const godot::Ref<sm::Attribute> itemToCompare = m_gdAttributes[i];

		if (itemToCompare.is_null() || itemToCompare->GetName().is_empty() || newName.is_empty())
		{
			continue;
		}

		if (itemToCompare->GetName() == newName)
		{
			godot::UtilityFunctions::push_warning(godot::String("Duplicate attribute: ") + newName);
		}
	}
}
