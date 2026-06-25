#include "core/TagSystem.h"

#include "godot/gdTagContainer.h"
#include "godot/gdGASWorld.h"

#include <godot_cpp/classes/node.hpp>
#include <algorithm>

sm::TagSystem::TagSystem(GAS_World* w)
{
	_world = w;
}

void sm::TagSystem::Update(float dt)
{
	_OnContainerRemoved();
	_OnContainerAdded();

	_OnTagRemoved();
	_OnTagAdded();
}

void sm::TagSystem::RegisterTagContainer(TagContainer* container)
{
	container->OnTagAdded = [this](TagID id, TagContainer* node)
		{
			this->m_TagsToAdd.emplace_back(std::pair<TagID, TagContainer*>{ id, node });
		};

	container->OnTagRemoved = [this](TagID id, TagContainer* node)
		{
			this->m_TagsToRemove.emplace_back(std::pair<TagID, TagContainer*>{ id, node });
		};

	m_ContainersToAdd.push_back(container);
	m_TagContainers.Add(container);
}

void sm::TagSystem::UnregisterTagContainer(TagContainer* container)
{
	m_ContainersToRemove.push_back(container);
	m_TagContainers.Remove(container);
}

const std::vector<sm::TagContainer*>& sm::TagSystem::ContainersWithTag(TagID id)
{
	auto mapFound = m_ContainersWithTag.find(id);

	if (mapFound != m_ContainersWithTag.end())
	{
		return mapFound->second.iterable;
	}

	TagCache cache;
	cache.iterable.reserve(64);
	for (auto* container : m_TagContainers.iterable)
	{
		if (container->HasTag(id))
		{
			cache.Add(container);
		}
	}

	// Cache id
	auto [it, inserted] = m_ContainersWithTag.emplace(id, std::move(cache));
	return it->second.iterable;
}

const std::vector<sm::TagContainer*> sm::TagSystem::ContainersWithoutTag(TagID id)
{
	std::vector<sm::TagContainer*> ret;
	ret.reserve(64);

	for (auto* container : m_TagContainers.iterable)
	{
		if (!container->HasTag(id))
		{
			ret.push_back(container);
		}
	}

	return ret;
}

/// <summary>
/// 1. Get all containers that have any of those tags.
/// Save the tag they contain and the container.
/// 2. Sort by number of containers from smallest to largest.
/// To check All tags, the return value should be:
/// Container1 & Container2 & Container3 & ContainerN...
/// Hence checking the smallest container already guarantees that all tags must
/// be within those.
/// 3. Check if other tags are also in the smallest container. If they aren't, discard.
std::vector<sm::TagContainer*> sm::TagSystem::All(const godot::PackedInt32Array& tags)
{
	struct TagList
	{
		TagID id;
		std::vector<TagContainer*> containers;
	};

	std::vector<TagContainer*> retNodes;
	retNodes.reserve(64);

	std::vector<TagList> anyNodes;
	anyNodes.reserve(tags.size());

	for (size_t i = 0; i < tags.size(); i++)
	{
		TagID tagID = tags[i];
		auto result = m_ContainersWithTag.find(tagID);
		const std::vector<TagContainer*>& nodesWithTag = (result != m_ContainersWithTag.end()
			? result->second.iterable : ContainersWithTag(tagID));

		anyNodes.emplace_back(TagList{ tagID, nodesWithTag });
	}

	std::sort(anyNodes.begin(), anyNodes.end(),
		[](const TagList& a, const TagList& b)
		{
			return a.containers.size() < b.containers.size();
		});

	if (anyNodes.empty() || anyNodes[0].containers.empty())
	{
		return {};
	}

	for (TagContainer* container : anyNodes[0].containers)
	{
		bool has_all = true;

		// Ignore first Tag
		for (size_t i = 1; i < anyNodes.size(); i++)
		{
			if (!container->HasTag(anyNodes[i].id))
			{
				has_all = false;
				break;
			}
		}

		if (has_all)
		{
			retNodes.push_back(container);
		}
	}

	return retNodes;
}

std::vector<sm::TagContainer*> sm::TagSystem::None(const godot::PackedInt32Array& tags)
{
	std::vector<TagContainer*> retNodes;
	retNodes.reserve(64);

	for (TagContainer* container : m_TagContainers.iterable)
	{
		if (!container->HasAnyTag(tags))
		{
			retNodes.push_back(container);
		}
	}

	return retNodes;
}

std::vector<sm::TagContainer*> sm::TagSystem::Any(const godot::PackedInt32Array& tags)
{
	std::vector<TagContainer*> retNodes;
	retNodes.reserve(64);

	std::unordered_set<uint64> added;

	for (size_t i = 0; i < tags.size(); i++)
	{
		TagID tagID = tags[i];
		auto result = m_ContainersWithTag.find(tagID);

		const std::vector<TagContainer*>& nodesWithTag = (result != m_ContainersWithTag.end()
			? result->second.iterable : ContainersWithTag(tagID));

		for (TagContainer* node : nodesWithTag)
		{
			uint64 id = node->get_instance_id();

			if (added.insert(id).second)
			{
				retNodes.push_back(node);
			}
		}
	}

	return retNodes;
}

void sm::TagSystem::_OnContainerAdded()
{
	for (TagContainer* node : m_ContainersToAdd)
	{
		BitSet<MAX_TAGS> tags = node->GetTagSet().tags;

		for (size_t block = 0; block < tags.GetSize(); ++block)
		{
			// Get bits per block
			auto bits = tags[block];

			while (bits)
			{
				int bit = std::countr_zero(bits);	// Returns the number of consecutive 0 bits in the value of x (right)
				int index = block * 64 + bit;

				ERR_CONTINUE_MSG(index >= MAX_TAGS, godot::vformat("AddTags skipped: index %d out of range (MAX_TAGS=%d)", index, MAX_TAGS));

				if (auto found = m_ContainersWithTag.find(index);
					found != m_ContainersWithTag.end())
				{
					TagCache& cache = found->second;
					cache.Add(node);
				}
				else
				{
					m_ContainersWithTag[index].Add(node);
				}

				bits &= bits - 1;
			}
		}
	}

	m_ContainersToAdd.clear();
}

void sm::TagSystem::_OnContainerRemoved()
{
	for (TagContainer* node : m_ContainersToRemove)
	{
		BitSet<MAX_TAGS> tags = node->GetTagSet().tags;

		for (size_t block = 0; block < tags.GetSize(); ++block)
		{
			// Get bits per block
			auto bits = tags[block];

			while (bits)
			{
				int bit = std::countr_zero(bits);	// Returns the number of consecutive 0 bits in the value of x (right)
				int index = block * 64 + bit;

				ERR_CONTINUE_MSG(index >= MAX_TAGS, godot::vformat("RemoveTags skipped: index %d out of range (MAX_TAGS=%d)", index, MAX_TAGS));

				if (auto found = m_ContainersWithTag.find(index);
					found != m_ContainersWithTag.end())
				{
					TagCache& cache = found->second;
					cache.Remove(node);
				}

				bits &= bits - 1;
			}
		}
	}

	m_ContainersToRemove.clear();
}

void sm::TagSystem::_OnTagAdded()
{
	for (auto& [id, node] : m_TagsToAdd)
	{
		m_ContainersWithTag[id].Add(node);
	}

	m_TagsToAdd.clear();
}

void sm::TagSystem::_OnTagRemoved()
{
	for (auto& [id, node] : m_TagsToRemove)
	{
		m_ContainersWithTag[id].Remove(node);
	}

	m_TagsToRemove.clear();
}
