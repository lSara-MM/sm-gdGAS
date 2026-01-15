#pragma once
#include "Types.h"
#include "GameplayTag.h"
#include "GameplayAbilitySystemNode.h"

namespace sm
{
	class TagContainer final : public sm::GameplayAbilitySystem
	{
		GDCLASS(TagContainer, sm::GameplayAbilitySystem)

	public:
		TagContainer();
		~TagContainer();

		void _ready();
		void Test(godot::StringName name);

	protected:
		static void _bind_methods();

	private:

		// Activation Rules
		//std::vector<GameplayTag> m_AbiltyTags;
		//std::vector<GameplayTag> m_CancelAbiltyTags;
		//std::vector<GameplayTag> m_BlockAbiltyTags;
		//std::vector<GameplayTag> m_ActivationRequirementTags;
		//std::vector<GameplayTag> m_DeactivationRequirementTags;
	};
}