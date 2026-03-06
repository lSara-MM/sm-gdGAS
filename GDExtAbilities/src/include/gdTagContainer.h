#pragma once
#include "Types.h"
#include "GameplayTag.h"
#include "gdGameplayAbilitySystemNode.h"

namespace sm
{
	class TagContainer final : public sm::GameplayAbilitySystem
	{
		GDCLASS(TagContainer, sm::GameplayAbilitySystem)

	protected:
		static void _bind_methods();

	public:
		TagContainer();
		~TagContainer();

		void _ready();

		godot::TypedArray<TagID> GetTags() const { return m_gdTags; };
		void SetTags(const godot::TypedArray<TagID>& tags) { m_gdTags = tags; };

		void AddTag(TagID tag);
		void AddTags(const godot::TypedArray<TagID>& tags);
		void RemoveTag(TagID tag);
		void RemoveTags(const godot::TypedArray<TagID>& tags);

	private:
		godot::TypedArray<TagID> m_gdTags;
		std::vector<TagID> m_GameplayTags;
	};
}