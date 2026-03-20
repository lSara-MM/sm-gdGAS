#include "godot/gdGASEntity.h"

#include "godot/gdGASWorld.h"
#include "core/EffectSystem.h"
#include "core/GameplayEffect.h"

sm::GAS_Entity::GAS_Entity()
{}

sm::GAS_Entity::~GAS_Entity()
{
	m_AttrContainer = nullptr;
	m_TagContainer = nullptr;
}

void sm::GAS_Entity::SetID(EntityID id)
{
	if (m_ID == 0)
	{
		m_ID = id;
	}
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
	m_AttrContainer = nullptr;
	m_TagContainer = nullptr;
}

void sm::GAS_Entity::OnReady()
{
	sm::GAS_World* world = sm::GAS_World::Instance();

	if (!world)
	{
		queue_free();
		ERR_FAIL_MSG("Could not create Entity. GAS_World Node required");
	}

	m_ID = world->RegisterEntity(this);
	
	m_AttrContainer = godot::Object::cast_to<AttributeContainer>(get_node_or_null(attrContainerNodePath));
	m_TagContainer = godot::Object::cast_to<TagContainer>(get_node_or_null(tagContainerNodePath));
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
		godot::vformat("AddEffect: Could not add '%s'. The EffectSystem was not found.",
			gdEffect->GetName()));

	if (!HandleTags(gdEffect))
	{
		return;
	}

	godot::TypedArray<ModifierData> modifiers = gdEffect->GetModifiers();
	if (modifiers.is_empty())
	{
		return;
	}

	ERR_FAIL_NULL_MSG(m_AttrContainer,
		godot::vformat("AddEffect: Could not add '%s'. The AttributeContainer was not found.",
			gdEffect->GetName()));

	EffectData::Type type = gdEffect->GetEffectType();
	if (type == EffectData::Type::Permanent)
	{
		for (int64_t i = 0; i < modifiers.size(); i++)
		{
			godot::Ref<ModifierData> modifier = modifiers[i];
			m_AttrContainer->AddBaseModifier(modifier->GetTargetID(), modifier);
		}

		return;
	}

	GameplayEffect effect(
		gdEffect->GetName(),
		world->GetEffectSystem()->m_EffectsID.GenerateUID(),
		static_cast<GameplayEffect::Type>(type),
		m_ID,
		gdEffect->GetSourceID(),
		gdEffect->GetDuration()
	);

	for (int64_t i = 0; i < modifiers.size(); i++)
	{
		godot::Ref<ModifierData> modifier = modifiers[i];
		GameplayAttribute* attr = m_AttrContainer->FindAttribute(modifier->GetTargetID());
		ModifierID id = m_AttrContainer->AddModifier(attr, modifier);
		ModifierHandle handle = {
			id,
			attr->GetUID(),
			modifier->GetGameplayOperationType(),
			attr->GetModifiersCount(modifier->GetGameplayOperationType()) - 1
		};

		effect.AddModifier(handle);
	}

	world->GetEffectSystem()->AddActiveEffect(effect);
}

bool sm::GAS_Entity::HandleTags(const godot::Ref<sm::EffectData>& gdEffect)
{
	godot::TypedArray<TagData> tagsToAdd = gdEffect->GetTagsToAdd();
	godot::TypedArray<godot::StringName> tagsToRemove = gdEffect->GetTagsToRemove();

	if (!tagsToAdd.is_empty() || !tagsToRemove.is_empty())
	{
		ERR_FAIL_NULL_V_MSG(m_TagContainer, false,
			godot::vformat("AddEffect: Could not add '%s'. The TagContainer was not found.",
				gdEffect->GetName()));

		// Add
		for (int64_t i = 0; i < tagsToAdd.size(); i++)
		{
			m_TagContainer->AddTag(tagsToAdd[i]);
		}

		// Remove
		for (int64_t i = 0; i < tagsToRemove.size(); i++)
		{
			m_TagContainer->RemoveTag(tagsToRemove[i]);
		}
	}

	return true;
}

void sm::GAS_Entity::RemoveTags(const BitSet<TAG_BITSET_SIZE> tags)
{
	m_TagContainer->RemoveTags(tags);
}
