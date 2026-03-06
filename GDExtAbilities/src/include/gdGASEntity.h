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

		EntityID GetID() const { return m_Id; }

		AttributeContainer* GetAttributeContainer() const { return attrContainer; }
		void SetAttributeContainer(AttributeContainer* node) { attrContainer = node; };
		godot::NodePath GetAttributeContainerNodePath() const { return attrContainerNodePath; }
		void SetAttributeContainerNodePath(godot::NodePath path);
		
		TagContainer* GetTagContainer() const { return tagContainer; }
		void SetTagContainer(TagContainer* node) { tagContainer = node; };
		godot::NodePath GetTagContainerNodePath() const { return tagContainerNodePath; }
		void SetTagContainerNodePath(godot::NodePath path);

		void AddEffect(const godot::Ref<EffectData> gdEffect);

	private:
		void OnExitTree() override;
		void OnReady() override;

	public:
		godot::NodePath attrContainerNodePath = "";
		godot::NodePath tagContainerNodePath = "";
		
	private:
		EntityID m_Id;
		AttributeContainer* attrContainer = nullptr;
		TagContainer* tagContainer = nullptr;
	};
}