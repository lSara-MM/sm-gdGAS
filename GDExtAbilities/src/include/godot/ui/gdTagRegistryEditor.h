#pragma once
#ifdef TOOLS_ENABLED
#include <godot/gdTagRegistry.h>
#include <godot/gdTagData.h>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>

namespace godot
{
	class AcceptDialog;
	class CheckBox;
	class EditorResourcePicker;
	class Label;
	class LineEdit;
	class Tree;
	class TreeItem;
	class VBoxContainer;
	class VSplitContainer;
}

namespace sm
{
	class TagRegistryEditor : public godot::EditorPlugin
	{
		GDCLASS(TagRegistryEditor, godot::EditorPlugin)

	protected:
		static void _bind_methods();

	public:
		enum class ButtonId : int
		{
			ADD = 0,
			DELETE_ALL,
			DELETE_SELF,
			EDIT,

			MAX
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

		void CreateTreeBoxContainer();
		void CreateOrUpdateTree();
		void CreateTag(const godot::Ref<sm::TagData>& resource, godot::TreeItem* parent);

		void DeleteTree();
		void DeleteTag(const godot::Ref<sm::TagData>& resource, godot::TreeItem* parent);

	private:
		void AddTagButton(godot::TreeItem*& item);
		void DeleteTagButton(godot::TreeItem*& item);

		void SaveRegistryResource();

		void _OnRegistryResourceChanged(const godot::Ref<godot::Resource> resource);

		void _OnButtonClicked(godot::TreeItem* item, int column, int id, int mouseButtonIndex);

		void _OnItemEdited();

		void _OnCreateTagClicked(godot::LineEdit* newText, godot::TreeItem* parentItem, godot::AcceptDialog* menu);

		void _OnTagNameChanged(const godot::String& newText, godot::TreeItem* item, godot::Label* labelResult);

		void _OnDeleteTagClicked(godot::TreeItem* item, godot::CheckBox* checkbox = nullptr, godot::AcceptDialog* menu = nullptr);

		void AddToCache(const godot::String& tag);
		bool HasTagInCache(const godot::String& tag);

	public:
		const int realMaxTags = MAX_TAGS - 2;

	private:
		godot::VSplitContainer* m_MainSplit = nullptr;
		godot::VBoxContainer* m_TreeContainer = nullptr;
		godot::VBoxContainer* m_InfoContainer = nullptr;
		godot::Tree* m_Tree = nullptr;
		godot::EditorResourcePicker* m_Picker = nullptr;

		godot::Ref<TagData> m_TagRegistry;

		Icons m_Icons;
		const godot::String m_SettingsPath;

		std::vector<godot::String> m_TagsCache;

		bool m_WasActive = false;
		bool m_DontShowAgain = false;
	};
}

#endif // TOOLS_ENABLED