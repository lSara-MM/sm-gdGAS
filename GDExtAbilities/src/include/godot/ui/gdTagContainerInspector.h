//#pragma once
//#ifdef TOOLS_ENABLED
//#include "godot/gdAbilityData.h"
//#include "godot/gdEffectData.h"
//#include "godot/gdTagData.h"
//
//#include <godot_cpp/classes/editor_inspector_plugin.hpp>
//#include <godot_cpp/classes/editor_plugin.hpp>
//#include <godot_cpp/core/object_id.hpp>
//
//// TODO: Refactor inspector so its not a messy mix of data types
//
//namespace godot
//{
//	class BoxContainer;
//	class Button;
//	class Control;
//	class Tree;
//	class TreeItem;
//	class VBoxContainer;
//	class VSplitContainer;
//}
//
//namespace sm
//{
//	class TagContainer;
//	class TagRegistryEditor;
//
//	class TagContainerInspector : public godot::EditorInspectorPlugin
//	{
//		GDCLASS(TagContainerInspector, godot::EditorInspectorPlugin)
//
//	protected:
//		static void _bind_methods();
//
//	public:
//		enum class Inspect
//		{
//			CONTAINER,
//			EFFECT_ADD,
//			EFFECT_REMOVE,
//			ABILITY
//		};
//
//		bool _can_handle(godot::Object* object) const override;
//		void _parse_begin(godot::Object* object) override;
//		bool _parse_property(Object* object,
//			godot::Variant::Type type,
//			const godot::String& name,
//			godot::PropertyHint hintType,
//			const godot::String& hintString,
//			godot::BitField<godot::PropertyUsageFlags> usageFlags,
//			bool wide) override;
//
//		void SetEditorPlugin(TagRegistryEditor* editor);
//
//	private:
//		struct TagTreeContext
//		{
//			Inspect current = Inspect::CONTAINER;
//			godot::ObjectID ownerId;
//			godot::Ref<EffectData> effect;
//			godot::Ref<AbilityData> ability;
//
//			godot::Tree* availableTree = nullptr;
//			godot::Tree* m_SelectedTree = nullptr;
//			godot::String search;
//			bool showEnabled = true;
//			bool showDisabled = true;
//			std::vector<godot::TreeItem*> visibleItems;
//			std::unordered_map<godot::StringName, godot::TreeItem*> itemsByName;
//		};
//
//		void ShowTagTreeEditor(Inspect current, godot::Object* object, const godot::String& title);
//		void AddSearchControls(godot::BoxContainer* root, godot::Control* gui, TagTreeContext& context);
//		int AddAvailableTagsTree(godot::VBoxContainer* root, TagTreeContext& context);
//		void AddSelectedTagsTree(godot::Control* gui, godot::VSplitContainer* mainSplit, TagTreeContext& context);
//
//		void RefreshAvailableTree(TagTreeContext& context);
//		void RefreshTagVisibility(TagTreeContext& context, const godot::Ref<TagData>& tag, godot::TreeItem* item);
//		std::vector<TagID>SetCheckbox(TagTreeContext& context, godot::TreeItem* item, bool value = true);
//
//		godot::PackedInt32Array GetTags(const TagTreeContext& context) const;
//		void SetTags(const TagTreeContext& context, const godot::PackedInt32Array& tags);
//		bool HasTag(const TagTreeContext& context, TagID id) const;
//		void AddTag(TagTreeContext& context, TagID id);
//		void RemoveTag(TagTreeContext& context, TagID id);
//
//		TagTreeContext* GetContext(godot::Tree* tree);
//		TagContainer* GetContainer(const TagTreeContext& context) const;
//
//		void DeleteTags(const std::vector<godot::Ref<TagData>>& tags);
//
//		void _OnCheckboxChanged(godot::Tree* tree);
//		void _OnAddButtonClicked(godot::Tree* tree);
//		void _OnRemoveButtonClicked(godot::TreeItem* item, int column, int id, int mouseButtonIndex, godot::Tree* tree);
//		void _OnSearchTextChanged(const godot::String& text, godot::Tree* tree);
//		void _OnItemActivated(godot::Tree* tree);
//		void _OnShowEnabledToggled(bool toggled, godot::Tree* tree);
//		void _OnShowDisabledToggled(bool toggled, godot::Tree* tree);
//
//	private:
//		TagRegistryEditor* editor = nullptr;
//		std::unordered_map<godot::Tree*, TagTreeContext> m_Contexts;
//		std::unordered_map<TagID, godot::StringName> m_IdToResource;
//#ifdef DEBUG_ENABLED
//		std::unordered_map<std::string, godot::TreeItem*> m_ItemsByNameDebug;
//#endif // DEBUG_ENABLED
//	};
//}
//
//#endif // TOOLS_ENABLED
