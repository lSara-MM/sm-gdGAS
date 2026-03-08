#include "gdGASEntity.h"

#include "gdGASWorld.h"
#include "EffectSystem.h"
#include "GameplayEffect.h"

sm::GAS_Entity::GAS_Entity()
{

}

void sm::GAS_Entity::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_attribute_container"), &GetAttributeContainer);
	godot::ClassDB::bind_method(godot::D_METHOD("set_attribute_container", "node"), &SetAttributeContainer);
	godot::ClassDB::bind_method(godot::D_METHOD("get_attribute_node_path"), &GetAttributeContainerNodePath);
	godot::ClassDB::bind_method(godot::D_METHOD("set_attribute_node_path", "path"), &SetAttributeContainerNodePath);

	godot::ClassDB::bind_method(godot::D_METHOD("get_tag_container"), &GetTagContainer);
	godot::ClassDB::bind_method(godot::D_METHOD("set_tag_container", "node"), &SetTagContainer);
	godot::ClassDB::bind_method(godot::D_METHOD("get_tag_node_path"), &GetTagContainerNodePath);
	godot::ClassDB::bind_method(godot::D_METHOD("set_tag_node_path", "path"), &SetTagContainerNodePath);

	godot::ClassDB::bind_method(godot::D_METHOD("add_effect", "effect"), &AddEffect);

	// Properties
	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::NODE_PATH,
		"attribute_container_node_path", godot::PROPERTY_HINT_NODE_PATH_VALID_TYPES, "AttributeContainer"),
		"set_attribute_node_path", "get_attribute_node_path"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::NODE_PATH,
		"tag_container_node_path", godot::PROPERTY_HINT_NODE_PATH_VALID_TYPES, "TagContainer"),
		"set_tag_node_path", "get_tag_node_path"
	);
}

void sm::GAS_Entity::OnExitTree()
{
	attrContainer = nullptr;
	tagContainer = nullptr;
}

void sm::GAS_Entity::OnReady()
{
	sm::GAS_World* world = sm::GAS_World::Instance();

	if (!world)
	{
		queue_free();
		ERR_FAIL_MSG("Could not create Entity. GAS_World Node required");
	}

	m_Id = world->RegisterEntity(this);
	
	attrContainer = godot::Object::cast_to<AttributeContainer>(get_node_or_null(attrContainerNodePath));
	tagContainer = godot::Object::cast_to<TagContainer>(get_node_or_null(tagContainerNodePath));
}

void sm::GAS_Entity::SetAttributeContainerNodePath(godot::NodePath path)
{
	attrContainerNodePath = path;
}

void sm::GAS_Entity::SetTagContainerNodePath(godot::NodePath path)
{
	tagContainerNodePath = path;
}

void sm::GAS_Entity::AddEffect(const godot::Ref<EffectData> gdEffect)
{
	sm::GAS_World* world = sm::GAS_World::Instance();
	
	ERR_FAIL_NULL_MSG(world,
		godot::vformat("AddEffect: Could not add %s. The EffectSystem was not found.",
			ToStdString(gdEffect->GetName()).c_str()));

	ERR_FAIL_NULL_MSG(tagContainer,
		godot::vformat("AddEffect: Could not add %s. The TagContainer was not found.",
			ToStdString(gdEffect->GetName()).c_str()));

	godot::TypedArray<TagID> tagsToAdd = gdEffect->GetTagsToAdd();
	for (size_t i = 0; i < tagsToAdd.size(); i++)
	{
		tagContainer->AddTag(tagsToAdd[i]);
	}

	godot::TypedArray<TagID> tagsToRemove = gdEffect->GetTagsToRemove();
	for (size_t i = 0; i < tagsToRemove.size(); i++)
	{
		tagContainer->RemoveTag(tagsToAdd[i]);
	}

	godot::TypedArray<ModifierData> modifiers = gdEffect->GetModifiers();
	if (modifiers.is_empty())
	{
		return;
	}

	ERR_FAIL_NULL_MSG(attrContainer,
		godot::vformat("AddEffect: Could not add %s. The AttributeContainer was not found.",
			ToStdString(gdEffect->GetName()).c_str()));

	EffectData::Type type = gdEffect->GetEffectType();
	if (type == EffectData::Type::Permanent)
	{
		for (size_t i = 0; i < modifiers.size(); i++)
		{
			godot::Ref<ModifierData> modifier = modifiers[i];
			attrContainer->AddBaseModifier(modifier->GetTargetID(), modifier);
		}

		return;
	}

	auto effect = std::make_unique<GameplayEffect>(
		gdEffect->GetName(),
		static_cast<GameplayEffect::Type>(gdEffect->GetEffectType()),
		m_Id,
		gdEffect->GetSourceID(),
		gdEffect->GetDuration()
	);

	for (size_t i = 0; i < modifiers.size(); i++)
	{
		godot::Ref<ModifierData> modifier = modifiers[i];
		GameplayAttribute* attr = attrContainer->FindAttribute(modifier->GetTargetID());
		ModifierID id = attrContainer->AddModifier(attr, modifier);
		ModifierHandle handle = {
			id,
			attr->GetUID(),
			modifier->GetGameplayOperationType(),
			attr->GetModifiersCount(modifier->GetGameplayOperationType()) - 1
		};

		effect->AddModifier(handle);
	}

	world->GetEffectSystem()->AddActiveEffect(effect);
}