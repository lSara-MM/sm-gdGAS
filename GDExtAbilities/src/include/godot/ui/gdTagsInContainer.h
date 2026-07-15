#pragma once
#ifdef TOOLS_ENABLED
#include "godot/ui/gdTagsInInspector.h"
#include "godot/gdTagData.h"

namespace sm
{
	class TagsInContainer : public TagsInInspector
	{
		GDCLASS(TagsInContainer, TagsInInspector)

	protected:
		static void _bind_methods() {};

	public:
		bool _can_handle(godot::Object* object) const override;
		void _parse_begin(godot::Object* object) override;
		bool _parse_property(Object* object,
			godot::Variant::Type type,
			const godot::String& name,
			godot::PropertyHint hintType,
			const godot::String& hintString,
			godot::BitField<godot::PropertyUsageFlags> usageFlags,
			bool wide) override;

		void SetCustomProperties(godot::VBoxContainer* root, godot::Control* gui, godot::VSplitContainer* mainSplit, const int tagsSize) override;
		void AddSearchControls(godot::BoxContainer* root, godot::Control* gui) override;
		void AddTagButtonToContainer(godot::VBoxContainer* root, godot::Control* gui, godot::VSplitContainer* mainSplit);
		void AddSelectedTagsTree(godot::Control* gui, godot::VSplitContainer* mainSplit);
		void RefreshTagVisibility(const godot::Ref<TagData>& tag, godot::TreeItem* item) override;

		void AddTag(TagID id) override;
		void RemoveTag(TagID id) override;

		godot::PackedInt32Array GetTags() const override;
		bool HasTag(TagID id) const override;

	private:
		void _OnAddButtonClicked();
		void _OnRemoveButtonClicked(godot::TreeItem* item, int column, int id, int mouseButtonIndex);
		void _OnItemActivated(godot::Tree* tree) override;
		void _OnSearchTextChanged(const godot::String& text) override;

	private:
		TagContainer* m_Container = nullptr;
		godot::Tree* m_SelectedTree = nullptr;
	};
}

#endif // TOOLS_ENABLED