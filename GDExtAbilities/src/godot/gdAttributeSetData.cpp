#include "godot/gdAttributeSetData.h"

#include <algorithm>
#include <unordered_set>
#include <godot_cpp/variant/utility_functions.hpp>

void sm::AttributeSetData::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_attributes_set"), &GetAttributesSet);
	godot::ClassDB::bind_method(godot::D_METHOD("set_attributes_set", "attr"), &SetAttributesSet);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::ARRAY,
		"attributes",
		godot::PROPERTY_HINT_RESOURCE_TYPE,
		"24/17:AttributeData"),
		"set_attributes_set", "get_attributes_set");

	godot::ClassDB::bind_method(godot::D_METHOD("add_attribute", "attr"), &AddAttribute);
}

void sm::AttributeSetData::SetAttributesSet(const godot::TypedArray<AttributeData>& attr)
{
	m_gdAttributes = attr;

#ifdef TOOLS_ENABLED
	for (int64_t i = 0; i < attr.size(); i++)
	{
		const godot::Ref<AttributeData> attribute = attr[i];

#ifdef DEBUG_ENABLED
		if (attribute.is_null())
		{
			m_DebugNames.push_back("<empty>");
		}
		else
		{
			m_DebugNames.push_back(ToStdString(attribute->GetName()));
		}
#endif //  DEBUG_ENABLED

		if (attribute.is_null())
		{
			continue;
		}

		attribute->eventSetName.SubscribeEvent([this](const godot::StringName& new_name)
			{
				this->_OnAttributeSetName(new_name);
			});
	}
#endif // TOOLS_ENABLED
}

void sm::AttributeSetData::AddAttribute(const godot::Ref<AttributeData>& attr)
{
	if (!m_gdAttributes.has(attr))
	{
		m_gdAttributes.push_back(attr);
		emit_changed();
	}
}

godot::Ref<sm::AttributeData> sm::AttributeSetData::GetAttributeResource(const godot::StringName& name) const
{
	godot::Ref<AttributeData> ref;

	for (int64_t i = 0; i < m_gdAttributes.size(); i++)
	{
		ref = m_gdAttributes[i];
		if (ref->GetName() == name)
		{
			return ref;
		}
	}

	return godot::Ref<AttributeData>();
}

std::vector<sm::AttributeData> sm::AttributeSetData::ToAttributeVector()
{
	std::vector<AttributeData> ret;

	for (int64_t i = 0; i < m_gdAttributes.size(); ++i)
	{
		godot::Ref<AttributeData> ref = m_gdAttributes[i];
		ret.emplace_back(ref->GetBaseValue());
	}

	return ret;
}

std::vector<godot::Ref<sm::AttributeData>> sm::AttributeSetData::ToRefAttributeVector()
{
	std::vector<godot::Ref<AttributeData>> ret;
	ret.reserve(m_gdAttributes.size());

	for (int64_t i = 0; i < m_gdAttributes.size(); ++i)
	{
		ret.emplace_back(m_gdAttributes[i]);
	}

	return ret;
}

std::vector<godot::Ref<sm::AttributeData>> sm::AttributeSetData::SortByName()
{
	std::vector<godot::Ref<AttributeData>> ret = ToRefAttributeVector();

	ret.erase(
		std::remove_if(ret.begin(), ret.end(),
			[](const godot::Ref<AttributeData>& attr)
			{
				return attr.is_null();
			}),
		ret.end()
	);

	std::sort(ret.begin(), ret.end(),
		[](const godot::Ref<AttributeData>& a, const godot::Ref<AttributeData>& b)
		{
			return a->GetName() < b->GetName();
		});

	return ret;
}

godot::TypedArray<sm::AttributeData> sm::AttributeSetData::ValidateSetData(const godot::TypedArray<AttributeData>& attr, bool getNull)
{
	godot::TypedArray<AttributeData> validatedSet;
	std::unordered_set<godot::StringName> seen;

	for (int64_t i = 0; i < attr.size(); i++)
	{
		const godot::Ref<AttributeData> itemToCompare = attr[i];

		if (itemToCompare.is_null())
		{
			if (getNull)
			{
				validatedSet.push_back(itemToCompare);
			}
			continue;
		}

		const godot::StringName name = itemToCompare->GetName();
		if (!name.is_empty() && seen.find(name) != seen.end())
		{
			continue;
		}

		seen.emplace(name);
		validatedSet.push_back(itemToCompare);
	}

	return validatedSet;
}

void sm::AttributeSetData::_OnAttributeSetName(const godot::StringName& newName)
{
	for (int64_t i = 0; i < m_gdAttributes.size(); i++)
	{
		const godot::Ref<AttributeData> itemToCompare = m_gdAttributes[i];

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