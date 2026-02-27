#include "gdAttributeContainer.h"

#include "GameplayAttribute.h"
#include "GameplayAttributeSet.h"
#include "GameplayModifier.h"
#include "gdAttribute.h"
#include "gdGASWorld.h"

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

	// Properties
	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::OBJECT,
		"attribute_set", godot::PROPERTY_HINT_RESOURCE_TYPE, "AttributeSetData"),
		"set_attributes_set", "get_attributes_set"
	);

	// Signals
	ADD_SIGNAL(godot::MethodInfo("attribute_modified",
		godot::PropertyInfo(godot::Variant::OBJECT, "owner", godot::PROPERTY_HINT_NODE_TYPE, "AttributeContainer"),
		godot::PropertyInfo(godot::Variant::STRING_NAME, "attribute_name"),
		godot::PropertyInfo(godot::Variant::FLOAT, "old_value"),
		godot::PropertyInfo(godot::Variant::FLOAT, "new_value")
	));

	ADD_SIGNAL(godot::MethodInfo("modifier_added",
		godot::PropertyInfo(godot::Variant::OBJECT, "owner", godot::PROPERTY_HINT_NODE_TYPE, "AttributeContainer"),
		godot::PropertyInfo(godot::Variant::STRING_NAME, "attribute_name"),
		godot::PropertyInfo(godot::Variant::OBJECT, "modifier_data")
	));

	ADD_SIGNAL(godot::MethodInfo("modifier_removed",
		godot::PropertyInfo(godot::Variant::OBJECT, "owner", godot::PROPERTY_HINT_NODE_TYPE, "AttributeContainer"),
		godot::PropertyInfo(godot::Variant::STRING_NAME, "attribute_name"),
		godot::PropertyInfo(godot::Variant::OBJECT, "modifier_data")
	));

	ADD_SIGNAL(godot::MethodInfo("effect_added",
		godot::PropertyInfo(godot::Variant::OBJECT, "owner", godot::PROPERTY_HINT_NODE_TYPE, "AttributeContainer"),
		godot::PropertyInfo(godot::Variant::STRING_NAME, "attribute_name"),
		godot::PropertyInfo(godot::Variant::OBJECT, "effect_data")
	));

	ADD_SIGNAL(godot::MethodInfo("effect_removed",
		godot::PropertyInfo(godot::Variant::OBJECT, "owner", godot::PROPERTY_HINT_NODE_TYPE, "AttributeContainer"),
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

	m_gdAttributeSetData->ValidateSetData(m_gdAttributeSetData->GetAttributesSet());

	std::vector<godot::Ref<AttributeData>> attrs = m_gdAttributeSetData->SortByName();

	for (size_t i = 0; i < attrs.size(); ++i)
	{
		AddAttribute(attrs[i]->GetName(), attrs[i]->GetBaseValue());
	}
}

godot::Ref<sm::Attribute> sm::AttributeContainer::GetAttribute(AttributeID id) const
{
	return godot::Ref<Attribute>();
}

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

ModifierID sm::AttributeContainer::AddModifier(GameplayAttribute* attr, godot::Ref<ModifierData> mod)
{
	ModifierID modID = attr->AddModifier(mod).id;

	emit_signal("modifier_added", this, attr->GetUID(), mod);

	return modID;
}

ModifierID sm::AttributeContainer::AddModifier(AttributeID id, godot::Ref<ModifierData> mod)
{
	GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
	ERR_FAIL_NULL_V_MSG(attr, 0, godot::vformat("Attribute not found: %s", ToStdString(id).c_str()));

	ModifierID modID = attr->AddModifier(mod).id;

	emit_signal("modifier_added", this, id, mod);

	return modID;
}

void sm::AttributeContainer::AddBaseModifier(AttributeID id, godot::Ref<ModifierData> mod)
{
	GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
	ERR_FAIL_NULL_MSG(attr, godot::vformat("Attribute not found: %s", ToStdString(id).c_str()));
	attr->AddBaseModifier(mod);

	emit_signal("modifier_added", this, id, mod);
}

void sm::AttributeContainer::RemoveModifier(AttributeID id, godot::Ref<ModifierData> mod)
{
	GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
	attr->RemoveModifier(mod);
	ERR_PRINT("added");

	emit_signal("modifier_removed", this, id, mod);
}

//void sm::AttributeContainer::AddEffect(EntityID id, godot::Ref<EffectData> effect)
//{
//	//GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
//	GAS_World* world = GAS_World::GetSingleton();
//	GAS_Entity* entity = world->GetEntity(id);
//	EffectSystem* effectSystem = world->GetEffectSystem();
//
//	effectSystem->AddEffect(effect, entity);
//}

//void sm::AttributeContainer::RemoveEffect(AttributeID id, godot::Ref<EffectData> effect)
//{
//	GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
//
//	godot::TypedArray<ModifierData> modifiers = effect->GetModifiers();
//	for (size_t i = 0; i < modifiers.size(); i++)
//	{
//		emit_signal("modifier_removed", this, id, modifiers[i]);
//	}
//
//	m_EffectSystemPtr->RemoveEffect(*attr, effect);
//	emit_signal("effect_removed", this, id, effect);
//}
//
//void sm::AttributeContainer::RemoveEffect(AttributeID id, EffectID effectID)
//{
//	GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
//	auto effect = m_EffectSystemPtr->FindEffect(effectID);
//
//	// TODO: modifier_removed signal
//	/*godot::TypedArray<ModifierData> modifiers = attr->GetModifiers();
//	for (size_t i = 0; i < modifiers.size(); i++)
//	{
//		emit_signal("modifier_removed", this, id, modifiers[i]);
//	}*/
//
//	m_EffectSystemPtr->RemoveEffect(*attr, effectID);
//}

void sm::AttributeContainer::AddAttribute(godot::StringName id, float baseValue)
{
	GameplayAttribute* addedAttr = &m_AttributeSetPtr->AddAttribute(id, baseValue);
	m_AttributesByName.try_emplace(id, addedAttr);
}

void sm::AttributeContainer::ModifyAttribute(AttributeID id, float newValue)
{
	GameplayAttribute* attr = m_AttributeSetPtr->FindAttribute(id);
	float oldValue = attr->GetBase();
	attr->SetBase(newValue);

	emit_signal("attribute_modified", this, id, oldValue, newValue);
}