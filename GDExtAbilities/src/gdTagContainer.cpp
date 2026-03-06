#include "gdTagContainer.h"

#include "TagRegistry.h"

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
		godot::PROPERTY_HINT_NONE,
		"StringName"),
		"set_tags", "get_tags"
	);
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

void sm::TagContainer::AddTag(TagID tag)
{
	m_gdTags.push_back(tag);
	m_GameplayTags.push_back(tag);
}

void sm::TagContainer::AddTags(const godot::TypedArray<TagID>& tags)
{
	size_t size = tags.size();
	m_GameplayTags.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		m_gdTags.push_back(tags[i]);
		m_GameplayTags.push_back(tags[i]);
	}
}

void sm::TagContainer::RemoveTag(TagID tag)
{
	m_GameplayTags.erase(
		std::remove_if(m_GameplayTags.begin(), m_GameplayTags.end(),
			[&](const TagID& id)
			{
				return tag == id;
			}
		),
		m_GameplayTags.end()
	);
}

void sm::TagContainer::RemoveTags(const godot::TypedArray<TagID>& tags)
{
	m_GameplayTags.erase(
		std::remove_if(m_GameplayTags.begin(), m_GameplayTags.end(),
			[&](const TagID& id)
			{
				return tags.has(id);
			}
		),
		m_GameplayTags.end()
	);
}
