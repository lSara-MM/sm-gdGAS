#pragma once
#ifdef TOOLS_ENABLED
#include "godot/ui/gdTagInspector.h"
#include "godot/gdTagData.h"
#include "internal/Event.h"

#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <unordered_set>

namespace godot
{
	class AcceptDialog;
	class CheckBox;
	class EditorFileSystem;
	class EditorResourcePicker;
	class FileSystemDock;
	class Label;
	class LineEdit;
	class ProjectSettings;
	class Tree;
	class TreeItem;
	class VBoxContainer;
	class VSplitContainer;
}

namespace sm
{
	class TagContainer;

	class TagRegistryEditor : public godot::EditorPlugin
	{
		GDCLASS(TagRegistryEditor, godot::EditorPlugin)

	protected:
		static void _bind_methods();

	public:
		enum class ButtonId : int
		{
			Add = 0,
			DeleteAll,
			DeleteSelf,
			Edit,

			Max
		};

		struct Icons
		{
			godot::Ref<godot::Texture2D> add;
			godot::Ref<godot::Texture2D> edit;
			godot::Ref<godot::Texture2D> remove;
			godot::Ref<godot::Texture2D> removeInternal;
		};

	public:
		TagRegistryEditor();
		~TagRegistryEditor() = default;

		void _enter_tree() override;
		void _exit_tree() override;

		void _make_visible(bool visible) override;

		const std::vector<godot::Ref<TagData>> GetTags() const;

		void GenerateConstants();
		void _TagAddedToContainer(TagID id, const TagContainer* container);
		void _TagRemovedFromContainer(TagID id, const TagContainer* container);

		godot::TreeItem* GetRoot();

	private:
		void CreateTab();

		void CreateTreeBoxContainer();
		void CreateTagRegistryData(const godot::Ref<TagData>& resource);
		void CreateOrUpdateTree();

		godot::TreeItem* CreateRootItem();
		void CreateTag(const godot::Ref<TagData>& resource, godot::TreeItem* parent);
		void DeleteTree();

		void CreateInfoBoxContainer();

		void BindContainersSignals();

		// Disconnect signals
		void ClearTagData(godot::Ref<TagData>& resource);
		void AddTagButton(godot::TreeItem* item);
		void DeleteTagButton(godot::TreeItem* item);

		void SaveRegistryResource();

		void SetSetting(const char* settingPath, godot::String value);

		void _OnRegistryResourceChanged(const godot::Ref<godot::Resource>& resource);

		void _OnRegistryEdited();

		void _OnRegistryResourceSelected(const godot::Ref<godot::Resource>& resource, bool inspect);

		void _OnButtonClicked(godot::TreeItem* item, int column, int id, int mouseButtonIndex);

		void _OnItemSelected();

		//void _OnItemEdited();

		void _OnCreateTagClicked(godot::LineEdit* newText, godot::TreeItem* parentItem, godot::AcceptDialog* menu);

		void _OnCreateTagNameChanged(const godot::String& newText, godot::TreeItem* item, godot::Label* labelResult);

		void _OnDeleteTagClicked(godot::TreeItem* item, godot::CheckBox* checkbox = nullptr, godot::AcceptDialog* menu = nullptr);

		void _OnFileMoved(const godot::String& oldFile, const godot::String& newFile);
		void _OnFileRemoved(const godot::String& removedFile);
		void _OnFolderMoved(const godot::String& oldFolder, const godot::String& newFolder);

		void _OnReferenceActivated();

		void DeleteFromVector(const godot::Ref<TagData> resource);

		void AddToCache(const godot::StringName& tag);
		bool HasTagInCache(const godot::StringName& tag);
		void RemoveFromCache(const godot::Ref<TagData> tag);
		void RefreshTreeFromEditorChanges();

		godot::Ref<TagData> GetSelectedItem() const;

		bool IsNameValid(const godot::String& name) const;

	public:
		const int realMaxTags = MAX_TAGS - 2;
		const godot::String generatedPath = "res://gen/tags";
		Event<const std::vector<godot::Ref<TagData>>&> deleteTag;

	private:
		//
		godot::Ref<TagInspector> m_Inspector;

		//
		godot::VSplitContainer* m_MainSplit = nullptr;
		godot::VBoxContainer* m_TreeContainer = nullptr;
		godot::VBoxContainer* m_InfoContainer = nullptr;

		// Tree
		godot::Tree* m_Tree = nullptr;
		godot::TreeItem* m_AbilityName = nullptr;
		godot::EditorResourcePicker* m_Picker = nullptr;
		godot::FileSystemDock* m_FileSystemDock = nullptr;
		godot::ProjectSettings* m_ProjectSettings = nullptr;

		// Info container
		godot::Label* m_CurrentTagInfo = nullptr;
		godot::Label* m_ParentTagInfo = nullptr;
		godot::Label* m_ReferencesSize = nullptr;
		godot::Tree* m_ReferencesTree = nullptr;
		std::unordered_map<TagID, std::vector<uint64_t>> m_CurrentReferences;

		//
		godot::Ref<TagData> m_TagRegistry;

		Icons m_Icons;
		godot::String m_TagRegistryPath;

		std::vector<godot::Ref<TagData>> m_TagDatas;
		std::unordered_set<godot::StringName> m_TagsCache;
#ifdef DEBUG_ENABLED
		std::unordered_set<std::string> m_TagsCacheDebug;
#endif // DEBUG_ENABLED

		bool m_DontShowAgain = false;
		bool m_CanBeCreated = false;
	};
}
#endif // TOOLS_ENABLED
