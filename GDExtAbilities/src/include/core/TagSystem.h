#pragma once
#include "core/GameplaySystem.h"
#include "internal/Types.h"
#include "godot/gdTagData.h"

#include <unordered_map>
#include <utility>

namespace sm
{
	class TagContainer;

	class TagSystem : public GameplaySystem
	{
	public:
		TagSystem(GAS_World* w);
		void Update(float dt) override;

		void RegisterTagContainer(TagContainer* container);
		void UnregisterTagContainer(TagContainer* container);

		// Queries
		const std::vector<TagContainer*>& ContainersWithTag(TagID tag);
		const std::vector<TagContainer*> ContainersWithoutTag(TagID tag);

		std::vector<TagContainer*> All(const godot::PackedInt32Array& tags);
		std::vector<TagContainer*> None(const godot::PackedInt32Array& tags);
		std::vector<TagContainer*> Any(const godot::PackedInt32Array& tags);

	private:

		void _OnContainerAdded();
		void _OnContainerRemoved();

		void _OnTagAdded();
		void _OnTagRemoved();

	private:
		struct TagCache
		{
			std::vector<TagContainer*> iterable;
			std::unordered_map<TagContainer*, size_t> unique;

			void Add(TagContainer* node)
			{
				if (node)
				{
					if (unique.try_emplace(node, iterable.size()).second)
					{
						iterable.push_back(node);
					}
				}
			}

			void Remove(TagContainer* node)
			{
				if (auto found = unique.find(node);
					found != unique.end())
				{
					size_t lastIndex = iterable.size() - 1;
					size_t currentIndex = unique[node];
					TagContainer* lastContainer = iterable[lastIndex];

					unique[lastContainer] = currentIndex;

					std::swap(iterable[currentIndex], iterable[lastIndex]);

					iterable.pop_back();
					unique.erase(found);
				}
			}
		};

		TagCache m_TagContainers;

		std::vector<TagContainer*> m_ContainersToAdd;
		std::vector<TagContainer*> m_ContainersToRemove;

		std::vector<std::pair<TagID, TagContainer*>> m_TagsToAdd;
		std::vector<std::pair<TagID, TagContainer*>> m_TagsToRemove;

		// Cache
		std::unordered_map<TagID, TagCache> m_ContainersWithTag;
	};
}