#pragma once
#include "utils.h"
#include <godot_cpp/classes/node.hpp>

using namespace godot;

namespace sm
{
	class TagContainer final : public Node
	{
	public:
		TagContainer();
		~TagContainer();

	private:

		// Activation Rules
		std::vector<TagID> m_AbiltyTags;
		std::vector<TagID> m_CancelAbiltyTags;
		std::vector<TagID> m_BlockAbiltyTags;
		std::vector<TagID> m_ActivationRequirementTags;
		std::vector<TagID> m_DeactivationRequirementTags;
	};
}