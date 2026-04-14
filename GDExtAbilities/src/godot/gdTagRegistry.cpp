//#include "godot/gdTagRegistry.h"
//#include "core/TagRegistry.h"
//
//void sm::TagRegistryData::_bind_methods()
//{
//	godot::ClassDB::bind_method(godot::D_METHOD("get_tags"), &GetTags);
//	godot::ClassDB::bind_method(godot::D_METHOD("set_tags", "value"), &SetTags);
//
//	ADD_PROPERTY(godot::PropertyInfo(
//		godot::Variant::ARRAY,
//		"tags",
//		godot::PROPERTY_HINT_RESOURCE_TYPE,
//		"24/17:TagData"),
//		"set_tags", "get_tags"
//	);
//}
//
//void sm::TagRegistryData::SetTags(const godot::TypedArray<TagData>& value)
//{
//	m_Tags = value;
//}
//
//bool sm::TagRegistryData::HasChild(const godot::Ref<TagData>& tag, const godot::Ref<TagData>& tagChild) const
//{
//	TagRegistry& instance = TagRegistry::Instance();
//	auto gameplayTag = instance.FindGameplayTag(tag);
//
//	return gameplayTag->GetChildIndex(tagChild->GetInternalID()) != gameplayTag->GetChildren().size();
//}