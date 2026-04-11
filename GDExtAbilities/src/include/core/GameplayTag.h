#pragma once
#include "internal/Types.h"
#include "internal/smBitSet.h"

#include <vector>

namespace sm
{
	struct GameplayTag
	{

		GameplayTag(TagID id, godot::StringName name, TagID parent = ROOT_TAG) : m_UID(id), m_Name(name), m_ParentUID(parent)
		{
//#ifdef TOOLS_DEBUG_VS
			stdName = ToStdString(name);
//#endif //  TOOLS_DEBUG_VS
		};

		TagID GetUID() { return m_UID; };
		//const TagID GetUID() const { return static_cast<const TagID>(m_UID); };

		TagID GetParentUID() { return m_ParentUID; };
		//const TagID GetUID() const { return static_cast<const TagID>(m_UID); };

		godot::StringName GetName() { return m_Name; };

		std::vector<TagID>& GetChildren() { return m_Children; };

		size_t GetChildIndex(TagID id) const
		{
			for (size_t i = 0; i < m_Children.size(); i++)
			{
				if (m_Children[i] == id)
				{
					return i;
				}
			}

			return m_Children.size();
		}

	public:
		static constexpr TagID INVALID_TAG = 0;
		static constexpr TagID ROOT_TAG = 1;

	private:
		const TagID m_UID;
		TagID m_ParentUID;
		godot::StringName m_Name;

//#ifdef TOOLS_DEBUG_VS
		std::string stdName;
//#endif //  TOOLS_DEBUG_VS

		std::vector<TagID> m_Children;

		//BitSet<512> ascendantsMask;
		//BitSet<512> descendantsMask;
	};
}