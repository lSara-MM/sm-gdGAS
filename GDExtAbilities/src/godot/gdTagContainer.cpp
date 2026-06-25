#include "godot/gdTagContainer.h"

#include "core/TagRegistry.h"

#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/editor_settings.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

sm::TagContainer::TagContainer()
{}

sm::TagContainer::~TagContainer()
{}

void sm::TagContainer::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_tags"), &GetTags);
	godot::ClassDB::bind_method(godot::D_METHOD("set_tags"), &SetTags);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::PACKED_INT32_ARRAY, "tags"),
		"set_tags", "get_tags"
	);

	ADD_SIGNAL(godot::MethodInfo("tag_added",
		godot::PropertyInfo(godot::Variant::INT, "id"),
		godot::PropertyInfo(godot::Variant::OBJECT, "owner", godot::PROPERTY_HINT_NODE_TYPE, "TagContainer")
	));

	ADD_SIGNAL(godot::MethodInfo("tag_removed",
		godot::PropertyInfo(godot::Variant::INT, "id"),
		godot::PropertyInfo(godot::Variant::OBJECT, "owner", godot::PROPERTY_HINT_NODE_TYPE, "TagContainer")
	));
}

void sm::TagContainer::OnEnterTree()
{
	bool hasSibling = NodeUtils::HasSiblingOfType<TagContainer>(this);

	if (hasSibling)
	{
		if (!prevParent)
		{
			queue_free();
		}

		ERR_FAIL_MSG("Error: this node already has an existing TagContainer");
	}

	SetIniTags();
}

void sm::TagContainer::SetIniTags()
{
	for (size_t i = 0; i < m_gdTags.size(); i++)
	{
		SetTag(m_gdTags[i]);
	}
}

void sm::TagContainer::OnExitTree()
{
	prevParent = nullptr;
}

void sm::TagContainer::OnParented()
{
	bool hasSibling = NodeUtils::HasSiblingOfType<TagContainer>(this);

	if (hasSibling)
	{
		callable_mp(this, &TagContainer::RevertParenting).call_deferred();
	}
}

void sm::TagContainer::RevertParenting()
{
	if (!prevName.is_empty())
	{
		set_name(prevName);
	}

	if (prevParent)
	{
		reparent(prevParent);
	}
}

void sm::TagContainer::OnUnparented()
{
	prevParent = get_parent();
	prevName = get_name();
}

void sm::TagContainer::OnChildOrderChanged()
{
	WARN_PRINT_ED("TagContainer should not have children.");
}

godot::PackedInt32Array sm::TagContainer::GetTags() const
{
	return m_gdTags;
}

void sm::TagContainer::SetTags(const godot::PackedInt32Array& tags)
{
	m_gdTags = tags;
}

void sm::TagContainer::AddTag(TagID id)
{
	ERR_FAIL_COND_MSG(id == GameplayTag::INVALID_TAG, godot::vformat("AddTag failed: Unknown tag"));

	if (HasTag(id))
	{
		return;
	}

	bool ret = SetTag(id);
	if (ret)
	{
		m_gdTags.push_back(id);
	}
}

//void sm::TagContainer::AddTagFromPath(const godot::String& tag)
//{
//	TagRegistry& instance = TagRegistry::Instance();
//	TagID id = instance.FindTagID(tag);
//
//	ERR_FAIL_COND_MSG(id == GameplayTag::INVALID_TAG, godot::vformat("AddTag failed: Unknown tag '%s'", tag));
//
//	SetTag(id);
//	emit_signal("tag_added");
//}

void sm::TagContainer::RemoveTag(TagID id)
{
	ERR_FAIL_COND_MSG(id == GameplayTag::INVALID_TAG, godot::vformat("RemoveTag failed: Unknown tag"));

	bool ret = SetTag(id, false);
	if (ret)
	{
		if (int pos = m_gdTags.find(id); pos != -1)
		{
			m_gdTags.remove_at(pos);
		}
	}
}

//void sm::TagContainer::RemoveTagFromPath(const godot::String& tag)
//{
//	TagRegistry& instance = TagRegistry::Instance();
//	TagID id = instance.FindTagID(tag);
//
//	ERR_FAIL_COND_MSG(id == GameplayTag::INVALID_TAG, godot::vformat("RemoveTag failed: Unknown tag '%s'", tag));
//
//	SetTag(id, false);
//}

sm::TagSet sm::TagContainer::GetTagSet() const
{
	return m_TagsSet;
}

bool sm::TagContainer::HasTag(TagID id) const
{
	return m_TagsSet.tags.Has(id);
}

bool sm::TagContainer::HasAllTags(const godot::Array& tags) const
{
	TagRegistry& instance = TagRegistry::Instance();
	TagID id;

	for (int i = 0; i < tags.size(); ++i)
	{
		godot::Variant v = tags[i];
		id = GameplayTag::INVALID_TAG;

		switch (v.get_type())
		{
		case godot::Variant::STRING:
		case godot::Variant::STRING_NAME:
		{
			id = instance.FindTagID(v);
		}
		break;

		case godot::Variant::OBJECT:
		{
			godot::Object* obj = v;
			if (obj && obj->is_class("TagData"))
			{
				sm::TagData* res = godot::Object::cast_to<sm::TagData>(obj);
				id = res->GetInternalID();
			}
		} break;

		//case godot::Variant::INT:
		//{
		//	id = v;
		//}
		//break;

		default:
			ERR_FAIL_V_MSG(false, godot::vformat("HasAllTags failed: Unsupported tag type '%s'", v));
			break;
		}

		ERR_FAIL_COND_V_MSG(id == GameplayTag::INVALID_TAG, false, godot::vformat("HasAllTags failed: Unknown tag '%s'", v));

		if (!m_TagsSet.tags.Has(id))
		{
			return false;
		}
	}

	return true;
}

bool sm::TagContainer::HasAnyTag(const godot::Array& tags) const
{
	TagRegistry& instance = TagRegistry::Instance();
	TagID id;

	for (int i = 0; i < tags.size(); ++i)
	{
		godot::Variant v = tags[i];
		id = GameplayTag::INVALID_TAG;

		switch (v.get_type())
		{
		case godot::Variant::STRING:
		case godot::Variant::STRING_NAME:
		{
			id = instance.FindTagID(v);
		}
		break;

		case godot::Variant::OBJECT:
		{
			godot::Object* obj = v;
			if (obj && obj->is_class("TagData"))
			{
				sm::TagData* res = godot::Object::cast_to<sm::TagData>(obj);
				id = res->GetInternalID();
			}
		} break;

		//case godot::Variant::INT:
		//{
		//	id = v;
		//}
		//break;

		default:
			break;
		}

		ERR_FAIL_COND_V_MSG(id == GameplayTag::INVALID_TAG, false, godot::vformat("HasAnyTags failed: Unknown tag '%s'", v));

		if (m_TagsSet.tags.Has(id))
		{
			return true;
		}
	}

	return false;
}

bool sm::TagContainer::SetTag(TagID id, bool value)
{
	bool ret = false;

	ERR_FAIL_COND_V_MSG(id >= MAX_TAGS, ret, godot::vformat("SetTag failed: id %d out of range (MAX_TAGS=%d)", id, MAX_TAGS));

	uint16& count = m_TagsSet.stack[id];
	if (value)
	{
		if (++count == 1)
		{
			m_TagsSet.tags.Set(id, true);
		}

		ret = true;
		emit_signal("tag_added", id, this);
		notify_property_list_changed();

		if (OnTagAdded)
		{
			OnTagAdded(id, this);
		}
	}
	else
	{
		if (count > 0 && --count == 0)
		{
			m_TagsSet.tags.Set(id, false);
		}

		ret = true;
		emit_signal("tag_removed", id, this);
		notify_property_list_changed();

		if (OnTagRemoved)
		{
			OnTagRemoved(id, this);
		}
	}

	return ret;
}

void sm::TagContainer::AddTags(BitSet<MAX_TAGS> tags)
{
	for (size_t block = 0; block < tags.GetSize(); ++block)
	{
		// Get bits per block
		auto bits = tags[block];

		while (bits)
		{
			int bit = std::countr_zero(bits);	// Returns the number of consecutive 0 bits in the value of x (right)
			int index = block * 64 + bit;

			ERR_CONTINUE_MSG(index >= MAX_TAGS, godot::vformat("AddTags skipped: index %d out of range (MAX_TAGS=%d)", index, MAX_TAGS));

			SetTag(index);

			bits &= bits - 1;
		}
	}
}

void sm::TagContainer::RemoveTags(BitSet<MAX_TAGS> tags)
{
	for (size_t block = 0; block < tags.GetSize(); ++block)
	{
		// Get bits per block
		auto bits = tags[block];

		while (bits)
		{
			int bit = std::countr_zero(bits);	// Returns the number of consecutive 0 bits in the value of x (right)
			int index = block * 64 + bit;

			SetTag(index, false);

			bits &= bits - 1;
		}
	}
}