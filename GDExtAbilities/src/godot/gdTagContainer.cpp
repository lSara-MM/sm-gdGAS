#include "godot/gdTagContainer.h"

#include "core/TagRegistry.h"

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

sm::TagContainer::TagContainer()
{}

sm::TagContainer::~TagContainer()
{}

void sm::TagContainer::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_tags"), &GetTags);
	godot::ClassDB::bind_method(godot::D_METHOD("set_tags"), &SetTags);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::ARRAY,
		"tags",
		godot::PROPERTY_HINT_RESOURCE_TYPE,
		"24/17:TagData"),
		"set_tags", "get_tags"
	);

	ADD_SIGNAL(godot::MethodInfo("tag_added",
		godot::PropertyInfo(godot::Variant::OBJECT, "owner", godot::PROPERTY_HINT_NODE_TYPE, "TagContainer"),
		godot::PropertyInfo(godot::Variant::OBJECT, "tag", godot::PROPERTY_HINT_RESOURCE_TYPE, "tag_data")
	));

	ADD_SIGNAL(godot::MethodInfo("tag_removed",
		godot::PropertyInfo(godot::Variant::OBJECT, "owner", godot::PROPERTY_HINT_NODE_TYPE, "TagContainer"),
		godot::PropertyInfo(godot::Variant::OBJECT, "tag", godot::PROPERTY_HINT_RESOURCE_TYPE, "tag_data")
	));
}

void sm::TagContainer::_ready()
{
	/*{
		godot::UtilityFunctions::print("hola");
		TagRegistry& instance = TagRegistry::Instance();

		instance.RegisterTag("hola.tag");
		instance.RegisterTag("hola.tag.aaa");
		instance.RegisterTag("hola.tag.aaa");
		instance.RegisterTag("hola.tag.ccc");
		instance.RegisterTag("hola.TAG.aaa");
		instance.RegisterTag("hola.GA.bbb");
		instance.RegisterTag("hola.tag.hh");
		instance.RegisterTag("hola.45aa.hh");
		instance.RegisterTag("hola.33daa.hh");

		ERR_PRINT("PARENT");
		ERR_PRINT(instance.GetParent("bbb"));

		ERR_PRINT("TAGS");
		ERR_PRINT(instance.GetTag("bbb"));
		ERR_PRINT(instance.GetTag("TAG"));
		ERR_PRINT("Next should fail");
		ERR_PRINT(instance.GetTag("TAGa"));

		ERR_PRINT("CHILDREN");
		godot::TypedArray<godot::StringName> arr = instance.GetChildren("hola");
		for (size_t i = 0; i < arr.size(); i++)
		{
			ERR_PRINT(arr[i]);
		}

		ERR_PRINT("ASCENDANTS");
		godot::TypedArray<godot::StringName> arr2 = instance.GetAscendants("ccc");
		for (size_t i = 0; i < arr2.size(); i++)
		{
			ERR_PRINT(arr2[i]);
		}

		ERR_PRINT("DESCENDANTS");
		godot::TypedArray<godot::StringName> arr3 = instance.GetDescendants("hola");
		for (size_t i = 0; i < arr3.size(); i++)
		{
			ERR_PRINT(arr3[i]);
		}

		instance.UnregisterTag("hola.tag.hh");
		instance.UnregisterTag("hola.tag.ccc");
		instance.UnregisterTag("hola.tag.ccc");
		instance.UnregisterTag("hola.tag");

		ERR_PRINT("DESCENDANTS AFTER UNREGISTER");
		godot::TypedArray<godot::StringName> arr6 = instance.GetDescendants("hola");
		for (size_t i = 0; i < arr6.size(); i++)
		{
			ERR_PRINT(arr6[i]);
		}

		ERR_PRINT("CHILDREN");
		godot::TypedArray<godot::StringName> arr4 = instance.GetChildren("hola");
		for (size_t i = 0; i < arr3.size(); i++)
		{
			ERR_PRINT(arr3[i]);
		}
	}*/
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

void sm::TagContainer::AddTag(const godot::Ref<TagData>& tag)
{
	ERR_FAIL_COND_MSG(tag->GetInternalID() == GameplayTag::INVALID_TAG, godot::vformat("AddTag failed: Unknown tag '%s'", tag));

	SetTag(tag->GetInternalID());
}

void sm::TagContainer::AddTagFromPath(const godot::String& tag)
{
	TagRegistry& instance = TagRegistry::Instance();
	TagID id = instance.FindTagID(tag);

	ERR_FAIL_COND_MSG(id == GameplayTag::INVALID_TAG, godot::vformat("AddTag failed: Unknown tag '%s'", tag));

	SetTag(id);
}

void sm::TagContainer::RemoveTag(const godot::Ref<TagData>& tag)
{
	ERR_FAIL_COND_MSG(tag->GetInternalID() == GameplayTag::INVALID_TAG, godot::vformat("RemoveTag failed: Unknown tag '%s'", tag));

	SetTag(tag->GetInternalID(), false);
}

void sm::TagContainer::RemoveTagFromPath(const godot::String& tag)
{
	TagRegistry& instance = TagRegistry::Instance();
	TagID id = instance.FindTagID(tag);

	ERR_FAIL_COND_MSG(id == GameplayTag::INVALID_TAG, godot::vformat("RemoveTag failed: Unknown tag '%s'", tag));

	SetTag(id, false);
}

bool sm::TagContainer::HasTag(const godot::Ref<TagData>& tag) const
{
	return m_TagsSet.Has(tag->GetInternalID());
}

bool sm::TagContainer::HasTag(TagID id) const
{
	return m_TagsSet.Has(id);
}

bool sm::TagContainer::HasTagFromPath(const godot::String& tag) const
{
	TagRegistry& instance = TagRegistry::Instance();
	TagID id = instance.FindTagID(tag);

	return m_TagsSet.Has(id);
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

		if (!m_TagsSet.Has(id))
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

		if (m_TagsSet.Has(id))
		{
			return true;
		}
	}

	return false;
}

void sm::TagContainer::SetTag(TagID id, bool value)
{
	ERR_FAIL_COND_MSG(id >= MAX_TAGS, godot::vformat("SetTag failed: id %d out of range (MAX_TAGS=%d)", id, MAX_TAGS));

	uint16& count = m_TagsStack[id];
	if (value)
	{
		if (++count == 1)
		{
			m_TagsSet.Set(id, true);
		}

		emit_signal("tag_added");
	}
	else
	{
		if (count > 0 && --count == 0)
		{
			m_TagsSet.Set(id, false);
		}

		emit_signal("tag_removed");
	}
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