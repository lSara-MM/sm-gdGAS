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

	godot::ClassDB::bind_method(godot::D_METHOD("get_children"), &GetChildren);
	godot::ClassDB::bind_method(godot::D_METHOD("set_children", "value"), &SetChildren);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::STRING_NAME, "name"),
		"set_name", "get_name"
	);

	ADD_PROPERTY(godot::PropertyInfo(
		godot::Variant::STRING_NAME, "path"),
		"", "get_path"
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
	UpdateChildrenParents();
}

void sm::TagData::SetPath(const godot::String& value)
{
	m_TagFullPath = godot::String(value) + "." + m_Name;

	if (value.is_empty())
	{
		m_Path = ".";
	}
	else
	{
		m_Path = value;
	}

#ifdef DEBUG_ENABLED
	auto pathDebug = ToStdString(m_TagFullPath);
	ERR_PRINT(m_TagFullPath);
#endif // DEBUG_ENABLED
}

void sm::TagData::AddChild(godot::Ref<TagData> child)
{
	m_Children.push_back(child);
	UpdateChildrenParents();
}

void sm::TagData::RemoveChild(godot::Ref<TagData> child)
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
		godot::String root;
		if (m_Path == ".")
		{
			root = m_Path + m_Name;
		}
		else if (!m_Path.is_empty())
		{
			root = m_Path + "." + m_Name;
		}

		tagsStack.push_back(TagStackEntry{ m_Children[i], root });
		m_TagFullPath = root;
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

		tag->SetPath(tagEntry.hierarchy);

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
			auto pathDebug = ToStdString(root);
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