#pragma once
#include "core/GameplaySystem.h"
#include "internal/Types.h"
#include "godot/gdTagData.h"

#include <unordered_map>
#include <unordered_set>

namespace sm
{
	class TagContainer;

	class TagSystem : public GameplaySystem
	{
	public:
		void RegisterTagContainer(TagContainer* container);
		void UnregisterTagContainer(TagContainer* container);

		// Queries
		const std::vector<TagContainer*>& ContainersWithTag(const godot::Ref<TagData> tag);
		const std::vector<TagContainer*>& ContainersWithoutTag(const godot::Ref<TagData> tag);

		std::vector<TagContainer*> All(const godot::TypedArray<TagData>& tags);
		std::vector<TagContainer*> None(const godot::TypedArray<TagData>& tags);
		std::vector<TagContainer*> Any(const godot::TypedArray<TagData>& tags);

	private:
		const std::vector<TagContainer*>& ContainersWithTag(TagID tag);
		const std::vector<TagContainer*>& ContainersWithoutTag(TagID tag);

	private:
		std::unordered_set<TagContainer*> m_TagContainers;

		// Cache
		std::unordered_map<TagID, std::vector<TagContainer*>> m_ContainersWithTag;
		std::unordered_map<TagID, std::vector<TagContainer*>> m_ContainersWithoutTag;
	};
}