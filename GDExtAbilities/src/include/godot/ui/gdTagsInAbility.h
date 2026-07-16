#pragma once
#ifdef TOOLS_ENABLED
#include "godot/ui/gdTagsInInspector.h"
#include "godot/gdAbilityData.h"

namespace sm
{
	class TagsInAbility : public TagsInInspector
	{
		GDCLASS(TagsInAbility, TagsInInspector)

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

		godot::PackedInt32Array GetTags() const override;
		void SetTags(const godot::PackedInt32Array& tags);
		bool HasTag(TagID id) const override;
		void AddTag(TagID id) override;
		void RemoveTag(TagID id) override;

	private:
		void ShowAbilityID();
		void _OnCheckboxChanged() override;
		void _OnItemActivated(godot::Tree* tree) override;
		void _OnRemoveButtonClicked(godot::TreeItem* item, int column, int id, int mouseButtonIndex);

	public:
		enum class Type
		{
			Ability,
			Block,
			ActivationRequirements,
			ActivationBlocked,
			None
		} arrType;

	private:
		godot::Ref<AbilityData> m_Ability;
	};
}

#endif // TOOLS_ENABLED