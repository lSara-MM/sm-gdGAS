#pragma once
#include "gdGameplayAbilitySystemNode.h"
#include "Types.h"

#include "gdAttributeContainer.h"
#include "gdTagContainer.h"

namespace sm
{
	class GAS_Entity : public GameplayAbilitySystem
	{
		GDCLASS(GAS_Entity, GameplayAbilitySystem)

	protected:
		static void _bind_methods();

	public:
		GAS_Entity();

		void _notification(int notification);

		EntityID GetID() const { return m_Id; }
		AttributeContainer* GetAttributeContainer() const { return attrContainerNode; }

	private:
		void OnEnterTree();
		void OnExitTree();
		void OnReady();
		void OnProcess();

	public:
		AttributeContainer* attrContainerNode = nullptr;
		TagContainer* tagContainerNode = nullptr;

	private:
		EntityID m_Id;
	};
}