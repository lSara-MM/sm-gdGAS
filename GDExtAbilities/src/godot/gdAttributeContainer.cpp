#include "godot/gdAttributeContainer.h"

#include "core/GameplayAttribute.h"
#include "core/GameplayAttributeSet.h"
#include "core/GameplayModifier.h"
#include "godot/gdGASWorld.h"

#include <godot_cpp/variant/signal.hpp>
#include <godot_cpp/classes/engine.hpp>

sm::AttributeContainer::AttributeContainer()
{
	m_AttributeSetPtr = std::make_unique<sm::GameplayAttributeSet>();
}

sm::AttributeContainer::~AttributeContainer()
{

}

void sm::AttributeContainer::_bind_methods()
{
	// Methods
	godot::ClassDB::bind_method(godot::D_METHOD("get_attributes_set"), &GetAttributeSet);
	godot::ClassDB::bind_method(godot::D_METHOD("set_attributes_set", "attr"), &SetAttributeSet);

	godot::ClassDB::bind_method(godot::D_METHOD("add_modifier", "attribute_id", "modifier"), static_cast<ModifierID(AttributeContainer::*)(AttributeID, godot::Ref<ModifierData>)>(&AddModifier));
	godot::ClassDB::bind_method(godot::D_METHOD("remove_modifier", "attribute_id", "modifier"), &RemoveModifier);

	godot::ClassDB::bind_method(godot::D_METHOD("get_attribute_base_value", "attribute_id"), &GetAttributeBaseValue);

	godot::ClassDB::bind_method(godot::D_METHOD("get_attribute_current_value", "attribute_id"), &GetAttributeCurrentValue);

	godot::ClassDB::bind_method(godot::D_METHOD("is_attribute_min", "attribute_id"), &IsAttributeMin);
	godot::ClassDB::bind_method(godot::D_METHOD("is_attribute_max", "attribute_id"), &IsAttributeMax);
	godot::ClassDB::bind_method(godot::D_METHOD("is_attribute_dirty", "attribute_id"), &IsAttributeDirty);

	GDVIRTUAL_BIND(_pre_attribute_change, "attribute_id", "calculated_value");

	// Properties
	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::OBJECT,
		"attribute_set", godot::PROPERTY_HINT_RESOURCE_TYPE, "AttributeSetData"),
		"set_attributes_set", "get_attributes_set"
	);

	// Signals
	ADD_SIGNAL(godot::MethodInfo("attribute_changed",
		godot::PropertyInfo(godot::Variant::STRING_NAME, "attribute_name"),
		godot::PropertyInfo(godot::Variant::FLOAT, "new_value"),
		godot::PropertyInfo(godot::Variant::FLOAT, "old_value")
	));

	ADD_SIGNAL(godot::MethodInfo("modifier_added",
		godot::PropertyInfo(godot::Variant::STRING_NAME, "attribute_name"),
		godot::PropertyInfo(godot::Variant::OBJECT, "modifier_data")
	));

	ADD_SIGNAL(godot::MethodInfo("modifier_removed",
		godot::PropertyInfo(godot::Variant::STRING_NAME, "attribute_name"),
		godot::PropertyInfo(godot::Variant::OBJECT, "modifier_data")
	));

	ADD_SIGNAL(godot::MethodInfo("effect_added",
		godot::PropertyInfo(godot::Variant::STRING_NAME, "attribute_name"),
		godot::PropertyInfo(godot::Variant::OBJECT, "effect_data")
	));

	ADD_SIGNAL(godot::MethodInfo("effect_removed",
		godot::PropertyInfo(godot::Variant::STRING_NAME, "attribute_name"),
		godot::PropertyInfo(godot::Variant::OBJECT, "effect_data")
	));
}

void sm::AttributeContainer::OnReady()
{
	if (m_gdAttributeSetData == nullptr)
	{
		return;
	}

	auto attrs = m_gdAttributeSetData->ValidateSetData(m_gdAttributeSetData->GetAttributesSet());

	for (size_t i = 0; i < attrs.size(); ++i)
	{
		godot::Ref<AttributeData> attr = attrs[i];
		AddAttribute(attr->GetName(), attr);
	}
}

float sm::AttributeContainer::PreAttributeChange(AttributeID id, float calculated)
{
	float ret = calculated;

	if (GDVIRTUAL_IS_OVERRIDDEN(_pre_attribute_change))
	{
		GDVIRTUAL_CALL(_pre_attribute_change, id, calculated, ret);
	}

	return ret;
}

//godot::Ref<sm::Attribute> sm::AttributeContainer::GetAttribute(AttributeID id) const
//{
//	return godot::Ref<Attribute>();
//}

godot::Ref<sm::AttributeSetData> sm::AttributeContainer::GetAttributeSet() const
{
	return m_gdAttributeSetData;
}

void sm::AttributeContainer::SetAttributeSet(const godot::Ref<AttributeSetData>& attrSet)
{
	m_gdAttributeSetData = attrSet;
}

float sm::AttributeContainer::GetAttributeBaseValue(AttributeID id)
{
	sm::GameplayAttribute* attr = FindAttribute(id);

	ERR_FAIL_NULL_V_MSG(attr, 0, godot::vformat("Attribute not found: %s", ToStdString(id).c_str()));

	return attr->GetBase();
}

float sm::AttributeContainer::GetAttributeCurrentValue(AttributeID id)
{
	sm::GameplayAttribute* attr = FindAttribute(id);

	ERR_FAIL_NULL_V_MSG(attr, 0, godot::vformat("Attribute not found: %s", ToStdString(id).c_str()));

	return attr->GetCurrent();
}

bool sm::AttributeContainer::IsAttributeMin(AttributeID id)
{
	sm::GameplayAttribute* attr = FindAttribute(id);

	ERR_FAIL_NULL_V_MSG(attr, 0, godot::vformat("Attribute not found: %s", ToStdString(id).c_str()));

	return attr->IsMin();
}

bool sm::AttributeContainer::IsAttributeMax(AttributeID id)
{
	sm::GameplayAttribute* attr = FindAttribute(id);

	ERR_FAIL_NULL_V_MSG(attr, 0, godot::vformat("Attribute not found: %s", ToStdString(id).c_str()));

	return attr->IsMax();
}

bool sm::AttributeContainer::IsAttributeDirty(AttributeID id)
{
	sm::GameplayAttribute* attr = FindAttribute(id);

	ERR_FAIL_NULL_V_MSG(attr, 0, godot::vformat("Attribute not found: %s", ToStdString(id).c_str()));

	return attr->IsDirty();
}

sm::GameplayAttribute* sm::AttributeContainer::FindAttribute(AttributeID id) const
{
	return m_AttributeSetPtr->FindAttribute(id);;
}

ModifierID sm::AttributeContainer::AddModifier(GameplayAttribute* attr, const godot::Ref<ModifierData> mod)
{
	ModifierID modID = attr->AddModifier(mod).id;
	emit_signal("modifier_added", attr->GetUID(), mod);

	return modID;
}

ModifierID sm::AttributeContainer::AddModifier(AttributeID id, const godot::Ref<ModifierData> mod)
{
	GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
	ERR_FAIL_NULL_V_MSG(attr, 0, godot::vformat("Attribute not found: %s", ToStdString(id).c_str()));

	ModifierID modID = attr->AddModifier(mod).id;
	emit_signal("modifier_added", id, mod);

	return modID;
}

void sm::AttributeContainer::AddBaseModifier(AttributeID id, godot::Ref<ModifierData> mod)
{
#ifdef DEBUG_ENABLED
	auto i = ToStdString(id);
#endif // DEBUG_ENABLED

	GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
	ERR_FAIL_NULL_MSG(attr, godot::vformat("Attribute not found: %s", ToStdString(id).c_str()));
	attr->AddBaseModifier(mod);

	emit_signal("modifier_added", id, mod);
}

void sm::AttributeContainer::RemoveModifier(AttributeID id, godot::Ref<ModifierData> mod)
{
	GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
	ERR_FAIL_NULL_MSG(attr, godot::vformat("Attribute not found: %s", ToStdString(id).c_str()));
	attr->RemoveModifier(mod);

	emit_signal("modifier_removed", id, mod);
}

void sm::AttributeContainer::AddAttribute(AttributeID id, const godot::Ref<AttributeData> data)
{
	GameplayAttribute* addedAttr = &m_AttributeSetPtr->AddAttribute(id, data->GetBaseValue(), data->GetMinValue(), data->GetMaxValue());
	m_AttributesByName.try_emplace(id, addedAttr);

	addedAttr->SetPreAttributeChange([this, id](float calculated)
		{
			float ret = calculated;

			if (GDVIRTUAL_IS_OVERRIDDEN(_pre_attribute_change))
			{
				GDVIRTUAL_CALL(_pre_attribute_change, id, calculated, ret);
			}

			return ret;
		});

	addedAttr->SetAttributeChanged([this, id](float newVal, float oldVal)
		{
			emit_signal("attribute_changed", id, newVal, oldVal);
		});
}

void sm::AttributeContainer::ModifyAttribute(AttributeID id, float newValue)
{
	GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
	ERR_FAIL_NULL_MSG(attr, godot::vformat("Attribute not found: %s", ToStdString(id).c_str()));
	float oldValue = attr->GetBase();
	attr->SetBase(newValue);

	emit_signal("attribute_changed", id, newValue, oldValue);
}