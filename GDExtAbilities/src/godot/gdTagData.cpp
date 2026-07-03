#include "godot/gdTagData.h"
#include "core/GameplayTag.h"
#include "core/TagRegistry.h"

sm::TagData::TagData() : m_Path(), m_InternalID(GameplayTag::INVALID_TAG)
{}

void sm::TagData::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_name"), &GetName);
	godot::ClassDB::bind_method(godot::D_METHOD("set_name", "id"), &SetName);

	godot::ClassDB::bind_method(godot::D_METHOD("get_path"), &GetPath);
	godot::ClassDB::bind_method(godot::D_METHOD("set_path", "path"), &SetPath);

	godot::ClassDB::bind_method(godot::D_METHOD("get_internal_id"), &GetInternalID);
	godot::ClassDB::bind_method(godot::D_METHOD("set_internal_id", "internal_id"), &SetInternalID);

	godot::ClassDB::bind_method(godot::D_METHOD("get_children"), &GetChildren);
	godot::ClassDB::bind_method(godot::D_METHOD("set_children", "value"), &SetChildren);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::STRING_NAME, "name"),
		"set_name", "get_name"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::STRING_NAME, "path", godot::PROPERTY_HINT_NONE, "", godot::PROPERTY_USAGE_DEFAULT | godot::PROPERTY_USAGE_READ_ONLY),
		"set_path", "get_path"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::INT, "id", godot::PROPERTY_HINT_NONE, "", godot::PROPERTY_USAGE_STORAGE),
		"set_internal_id", "get_internal_id"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::ARRAY,
		"children",
		godot::PROPERTY_HINT_RESOURCE_TYPE,
		"24/17:TagData"),
		"set_children", "get_children"
	);
}

void sm::TagData::SetName(const godot::String& value)
{
	m_Name = value;
	m_DebugName = ToStdString(value);
	UpdateChildrenParents();
}

void sm::TagData::SetPath(const godot::String& value)
{
	m_Path = value;

	godot::String root;
	if (m_Path == ".")
	{
		root = m_Path + m_Name;
	}
	else if (!m_Path.is_empty())
	{
		root = m_Path + "." + m_Name;
	}
	else
	{
		root = "." + m_Name;
	}

	SetTagFullPath(root);
}

void sm::TagData::SetFullPath(const godot::String& value)
{
	if (value.is_empty() || value == ".")
	{
		m_Path = ".";
		SetTagFullPath(m_Path + m_Name);
	}
	else
	{
		m_Path = value;
		SetTagFullPath(m_Path + "." + m_Name);
	}
}

void sm::TagData::SetInternalID(TagID value)
{
	m_InternalID = value;
	emit_changed();
}

void sm::TagData::AddChild(const godot::Ref<TagData>& child)
{
	m_Children.push_back(child);
	UpdateChildrenParents();
}

void sm::TagData::RemoveChild(const godot::Ref<TagData>& child)
{
	m_Children.erase(child);
	//emit_changed();
}

void sm::TagData::SetTagFullPath(godot::String value)
{
	m_TagFullPath = value;
}

void sm::TagData::SetChildren(const godot::TypedArray<TagData>& value)
{
	m_Children = value;
	UpdateChildrenParents();
}

void sm::TagData::UpdateChildrenParents()
{
	emit_changed();

	godot::String root;
	if (m_Path == ".")
	{
		root = m_Path + m_Name;
	}
	else if (!m_Path.is_empty())
	{
		root = m_Path + "." + m_Name;
	}

	if (m_Children.is_empty())
	{
		return;
	}

	struct TagStackEntry
	{
		godot::Ref<TagData> tag;
		godot::String hierarchy;
	};

	std::vector<TagStackEntry> tagsStack;

	for (int64_t i = 0; i < m_Children.size(); i++)
	{
		godot::Ref<TagData> tagChild = m_Children[i];
		tagsStack.push_back(TagStackEntry{ tagChild, root });
	}

	while (!tagsStack.empty())
	{
		TagStackEntry tagEntry = tagsStack.back();
		tagsStack.pop_back();
		const godot::Ref<TagData> tag = tagEntry.tag;

		if (tag.is_null())
		{
			continue;
		}

#ifdef DEBUG_ENABLED
		auto pathDebug = ToStdString(tagEntry.hierarchy);
#endif // DEBUG_ENABLED

		tag->SetFullPath(tagEntry.hierarchy);

		for (int64_t i = 0; i < tag->GetChildren().size(); i++)
		{
			const godot::Ref<TagData> tagChild = tag->GetChildren()[i];

			if (tagChild.is_null())
			{
				continue;
			}

			godot::String root = tagEntry.hierarchy + "." + tagEntry.tag->GetName();
			tagsStack.emplace_back(tagChild, root);
#ifdef DEBUG_ENABLED
			auto rootDebug = ToStdString(root);
#endif // DEBUG_ENABLED
		}
	}
}

void sm::TagData::Reset()
{
	m_Name = "";
	m_Path = "";
	m_TagFullPath = godot::StringName();
	m_InternalID = GameplayTag::INVALID_TAG;

	m_Children.clear();

	emit_changed();
}