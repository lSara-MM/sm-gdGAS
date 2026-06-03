#pragma once
#include "internal/Types.h"
#include "internal/smBitSet.h"

#include <vector>

namespace sm
{
	struct GameplayTag
	{
		GameplayTag(TagID id, godot::StringName name, TagID parent = ROOT_TAG);

		TagID GetUID();
		TagID GetParentUID();

		godot::StringName GetName();

		std::vector<TagID>& GetChildren();
		void AddChild(TagID id);

		size_t GetChildIndex(TagID id) const;

		bool HasChild(TagID childID) const;
		bool HasDescendant(TagID childID) const;

	public:
		static constexpr TagID INVALID_TAG = 0;
		static constexpr TagID ROOT_TAG = 1;

		BitSet<512> ascendantsMask;

	private:
		const TagID m_UID;
		TagID m_ParentUID;
		godot::StringName m_Name;

#ifdef DEBUG_ENABLED
		std::string stdName;
#endif //  DEBUG_ENABLED

		std::vector<TagID> m_Children;
		//BitSet<512> descendantsMask;
	};
}