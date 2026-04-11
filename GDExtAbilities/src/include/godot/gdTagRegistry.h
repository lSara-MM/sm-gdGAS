#pragma once
#include "godot/gdGameplayAbilitySystemResource.h"
#include "godot/gdTagData.h"

namespace sm
{
	class TagRegistryData : public GameplayAbilitySystemResource
	{
		GDCLASS(TagRegistryData, GameplayAbilitySystemResource)

	protected:
		static void _bind_methods();

	public:
		TagRegistryData() = default;
		~TagRegistryData() = default;

		godot::TypedArray<TagData> GetTags() const { return m_Tags; };
		void SetTags(const godot::TypedArray<TagData>& value);

		/// Returns true if `tagID` is a direct child of `childID`.
		bool HasChild(const godot::Ref<TagData>& tag, const godot::Ref<TagData>& tagChild) const;

	private:
		godot::TypedArray<TagData> m_Tags;
	};
}