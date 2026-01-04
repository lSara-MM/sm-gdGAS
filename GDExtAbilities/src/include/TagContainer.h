#pragma once
#include "Types.h"
#include "GameplayTag.h"

#include <godot_cpp/classes/node.hpp>

namespace sm
{
	class TagContainer final : public godot::Node
	{
		GDCLASS(TagContainer, Node)

	public:
		TagContainer();
		~TagContainer();

		void _ready();
		void Test(godot::StringName name);

	protected:
		static void _bind_methods();

	private:

		// Activation Rules
		std::vector<GameplayTag> m_AbiltyTags;
		std::vector<GameplayTag> m_CancelAbiltyTags;
		std::vector<GameplayTag> m_BlockAbiltyTags;
		std::vector<GameplayTag> m_ActivationRequirementTags;
		std::vector<GameplayTag> m_DeactivationRequirementTags;
	};
}