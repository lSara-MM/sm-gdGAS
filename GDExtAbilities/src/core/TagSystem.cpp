#include "core/TagSystem.h"

#include "godot/gdTagContainer.h"

#include <godot_cpp/classes/node.hpp>
#include <algorithm>

void sm::TagSystem::RegisterTagContainer(TagContainer* container)
{
	m_TagContainers.emplace(container);
}

void sm::TagSystem::UnregisterTagContainer(TagContainer* container)
{
	m_TagContainers.erase(container);
}

const std::vector<sm::TagContainer*>& sm::TagSystem::ContainersWithTag(const godot::Ref<TagData> tag)
{
	return ContainersWithTag(tag->GetInternalID());
}

const std::vector<sm::TagContainer*>& sm::TagSystem::ContainersWithoutTag(const godot::Ref<TagData> tag)
{
	return ContainersWithoutTag(tag->GetInternalID());
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
/// </summary>
/// <param name="tags"></param>
/// <returns></returns>
std::vector<sm::TagContainer*> sm::TagSystem::All(const godot::TypedArray<TagData>& tags)
{
	struct TagList
	{
		TagID id;
		std::vector<TagContainer*> containers;
	};

	std::vector<TagContainer*> allNodes;
	allNodes.reserve(64);

	std::vector<TagList> anyNodes;
	anyNodes.reserve(tags.size());

	for (size_t i = 0; i < tags.size(); i++)
	{
		godot::Ref<TagData> tag = tags[i];
		auto result = m_ContainersWithTag.find(tag->GetInternalID());
		const std::vector<TagContainer*>& nodesWithTag = (result != m_ContainersWithTag.end()
			? result->second : ContainersWithTag(tag));

		anyNodes.emplace_back(TagList{ tag->GetInternalID(), nodesWithTag });
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
		for (size_t i = 1; i < anyNodes.size() - 1; i++)
		{
			if (!container->HasTag(anyNodes[i].id))
			{
				has_all = false;
				break;
			}
		}

		if (has_all)
		{
			allNodes.push_back(container);
		}
	}

	//std::vector<TagID> tagIDs;
	//tagIDs.reserve(tags.size());

	//for (size_t i = 0; i < tags.size(); i++)
	//{
	//	godot::Ref<TagData> tag = tags[i];
	//	tagIDs.push_back(tag->GetInternalID());
	//}

	//std::sort(tagIDs.begin(), tagIDs.end(),
	//	[&](const TagID a, const TagID b)
	//	{
	//		auto aTag = m_ContainersWithTag.find(a);
	//		const std::vector<TagContainer*>* nodesWithTagA = &(aTag != m_ContainersWithTag.end()
	//			? aTag->second : ContainersWithTag(aTag->first));

	//		auto bTag = m_ContainersWithTag.find(a);
	//		const std::vector<TagContainer*>* nodesWithTagB = &(bTag != m_ContainersWithTag.end()
	//			? bTag->second : ContainersWithTag(bTag->first));

	//		return nodesWithTagA < nodesWithTagB;
	//	});

	//for (auto tagID : tagIDs)
	//{
	//	if ()
	//	{

	//	}
	//}

	/*std::vector<TagContainer*> allNodes;
	allNodes.reserve(64);

	std::vector<const std::vector<TagContainer*>*> anyNodes;
	anyNodes.reserve(tags.size());

	for (size_t i = 0; i < tags.size(); i++)
	{
		godot::Ref<TagData> tag = tags[i];
		auto result = m_ContainersWithTag.find(tag->GetInternalID());
		const std::vector<TagContainer*>* nodesWithTag = &(result != m_ContainersWithTag.end()
			? result->second : ContainersWithTag(tag));

		anyNodes.push_back(nodesWithTag);
	}

	std::sort(anyNodes.begin(), anyNodes.end(),
		[](const std::vector<TagContainer*>* a, const std::vector<TagContainer*>* b)
		{
			return a->size() < b->size();
		});

	for (TagContainer* container : *anyNodes[0])
	{
		for (size_t i = 0; i < anyNodes.size(); i++)
		{
			container->HasTag(tags[i]);
		}
	}*/

	return allNodes;
}

std::vector<sm::TagContainer*> sm::TagSystem::None(const godot::TypedArray<TagData>& tags)
{
	return std::vector<TagContainer*>();
}

std::vector<sm::TagContainer*> sm::TagSystem::Any(const godot::TypedArray<TagData>& tags)
{
	std::vector<TagContainer*> anyNodes;
	anyNodes.reserve(64);

	std::unordered_set<uint64> added;

	for (size_t i = 0; i < tags.size(); i++)
	{
		godot::Ref<TagData> tag = tags[i];
		auto result = m_ContainersWithTag.find(tag->GetInternalID());

		const std::vector<TagContainer*>& nodesWithTag = (result != m_ContainersWithTag.end()
			? result->second : ContainersWithTag(tag));

		for (TagContainer* node : nodesWithTag)
		{
			uint64 id = node->get_instance_id();

			if (added.insert(id).second)
			{
				anyNodes.push_back(node);
			}
		}
	}

	return anyNodes;
}

const std::vector<sm::TagContainer*>& sm::TagSystem::ContainersWithTag(TagID id)
{
	auto mapFound = m_ContainersWithTag.find(id);

	if (mapFound != m_ContainersWithTag.end())
	{
		return mapFound->second;
	}

	std::vector<TagContainer*> nodes;
	for (auto* container : m_TagContainers)
	{
		if (container->HasTag(id))
		{
			nodes.push_back(container);
		}
	}

	// Cache id
	auto [it, inserted] = m_ContainersWithTag.emplace(id, std::move(nodes));
	return it->second;
}

const std::vector<sm::TagContainer*>& sm::TagSystem::ContainersWithoutTag(TagID id)
{
	auto mapFound = m_ContainersWithoutTag.find(id);

	if (mapFound != m_ContainersWithoutTag.end())
	{
		return mapFound->second;
	}

	std::vector<TagContainer*> nodes;
	nodes.reserve(64);

	for (auto* container : m_TagContainers)
	{
		if (!container->HasTag(id))
		{
			nodes.push_back(container);
		}
	}

	// Cache id
	auto [it, inserted] = m_ContainersWithTag.emplace(id, std::move(nodes));
	return it->second;
}
