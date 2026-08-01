#include "godot/gdGASEntity.h"

#include "godot/gdGASWorld.h"
#include "godot/gdAttributeContainer.h"
#include "godot/gdTagContainer.h"
#include "core/EffectSystem.h"
#include "core/GameplayEffect.h"
#include <godot_cpp/classes/engine.hpp>

sm::GAS_Entity::GAS_Entity()
{}

sm::GAS_Entity::~GAS_Entity()
{
	m_AttrContainer = nullptr;
	m_TagContainer = nullptr;
}

void sm::GAS_Entity::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_attribute_container"), &GetAttributeContainer);
	godot::ClassDB::bind_method(godot::D_METHOD("set_attribute_container", "node"), &SetAttributeContainer);
	godot::ClassDB::bind_method(godot::D_METHOD("get_attribute_node_path"), &GetAttributeContainerNodePath);

	godot::ClassDB::bind_method(godot::D_METHOD("get_tag_container"), &GetTagContainer);
	godot::ClassDB::bind_method(godot::D_METHOD("set_tag_container", "node"), &SetTagContainer);
	godot::ClassDB::bind_method(godot::D_METHOD("get_tag_node_path"), &GetTagContainerNodePath);

	godot::ClassDB::bind_method(godot::D_METHOD("add_effect", "effect"), &AddEffect);

	// Properties
	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::OBJECT,
		"attribute_container_node_path",
		godot::PROPERTY_HINT_NODE_TYPE,
		"AttributeContainer",
		godot::PROPERTY_USAGE_EDITOR | godot::PROPERTY_USAGE_READ_ONLY),
		"", "get_attribute_node_path"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::OBJECT,
		"tag_container_node_path",
		godot::PROPERTY_HINT_NODE_TYPE,
		"TagContainer",
		godot::PROPERTY_USAGE_EDITOR | godot::PROPERTY_USAGE_READ_ONLY),
		"", "get_tag_node_path"
	);
}

void sm::GAS_Entity::OnEnterTree()
{
	callable_mp(this, &GAS_Entity::Init).call_deferred();
}

void sm::GAS_Entity::OnExitTree()
{
	m_AttrContainer = nullptr;
	m_TagContainer = nullptr;

	sm::GAS_World* world = m_WorldBound.GetWorld(this);
	if (world)
	{
		world->UnregisterEntity(this);
	}

	m_WorldBound.CleanUp();
}

void sm::GAS_Entity::OnReady()
{
	m_AttrContainer = godot::Object::cast_to<AttributeContainer>(get_node_or_null(attrContainerNodePath));
	m_TagContainer = godot::Object::cast_to<TagContainer>(get_node_or_null(tagContainerNodePath));
}

void sm::GAS_Entity::OnChildOrderChanged()
{
	godot::TypedArray<godot::Node> children = get_children();
	bool hasAttr = false;
	bool hasTags = false;

	for (size_t i = 0; i < children.size(); i++)
	{
		if (auto* node = godot::Object::cast_to<AttributeContainer>(children[i]))
		{
			m_AttrContainer = node;
			SetAttributeContainerNodePath(get_path_to(node));
			hasAttr = true;
			continue;
		}

		if (auto* node = godot::Object::cast_to<TagContainer>(children[i]))
		{
			m_TagContainer = node;
			SetTagContainerNodePath(get_path_to(node));
			hasTags = true;
			continue;
		}
	}

	if (!hasAttr)
	{
		m_AttrContainer = nullptr;
		SetAttributeContainerNodePath("");
	}

	if (!hasTags)
	{
		m_TagContainer = nullptr;
		SetTagContainerNodePath("");
	}
}

void sm::GAS_Entity::Init()
{
	godot::Node* sceneRoot = NodeUtils::GetSceneRoot(this);

	if (!is_inside_tree() || !get_tree())
	{
		return;
	}

	sm::GAS_World* world = m_WorldBound.GetOrInitWorld(this, sceneRoot);

	if (!world)
	{
		if (!godot::Engine::get_singleton()->is_editor_hint())
		{
			queue_free();
			ERR_FAIL_MSG("Could not create Entity. GAS_World Node required.");
		}

		return;
	}

	m_ID = world->RegisterEntity(this);
}

void sm::GAS_Entity::SetID(EntityID id)
{
	if (m_ID != 0)
	{
		m_ID = id;
	}
}

sm::GAS_World* sm::GAS_Entity::GetWorld()
{
	godot::Node* sceneRoot = NodeUtils::GetSceneRoot(this);

	if (!is_inside_tree() || !get_tree())
	{
		return nullptr;
	}

	return m_WorldBound.GetOrInitWorld(this, sceneRoot);
}

void sm::GAS_Entity::SetAttributeContainerNodePath(godot::NodePath path)
{
	attrContainerNodePath = path;
}

void sm::GAS_Entity::SetTagContainerNodePath(godot::NodePath path)
{
	tagContainerNodePath = path;
}

EffectInstanceID sm::GAS_Entity::AddEffect(const godot::Ref<EffectData> gdEffect)
{
	sm::GAS_World* world = m_WorldBound.GetWorld(this);

	ERR_FAIL_NULL_V_MSG(world, 0,
		godot::vformat("AddEffect: Could not add '%s'. The EffectSystem was not found.",
			gdEffect->GetName()));

	if (!HandleTags(gdEffect))
	{
		return 0;
	}

	godot::TypedArray<ModifierData> modifiers = gdEffect->GetModifiers();
	if (modifiers.is_empty())
	{
		return 0;
	}

	ERR_FAIL_NULL_V_MSG(m_AttrContainer, 0,
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

		return 1;
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

	return world->GetEffectSystem()->AddActiveEffect(effect);
}

bool sm::GAS_Entity::HandleTags(const godot::Ref<EffectData>& gdEffect)
{
	godot::PackedInt32Array tagsToAdd = gdEffect->GetTagsToAdd();
	godot::PackedInt32Array tagsToRemove = gdEffect->GetTagsToRemove();

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

void sm::GAS_Entity::AddTags(const BitSet<MAX_TAGS> tags)
{
	m_TagContainer->AddTagsBitset(tags);
}

void sm::GAS_Entity::RemoveTags(const BitSet<MAX_TAGS> tags)
{
	m_TagContainer->RemoveTagsBitset(tags);
}
