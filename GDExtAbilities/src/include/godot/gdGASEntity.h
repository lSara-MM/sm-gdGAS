#pragma once
#include "godot/gdEffectData.h"
#include "godot/GASWorldBound.h"
#include "godot/gdGameplayAbilitySystemNode.h"
#include "internal/smBitSet.h"
#include "internal/Types.h"

#include <godot_cpp/variant/node_path.hpp>

namespace sm
{
	class AttributeContainer;
	class TagContainer;

	class GAS_Entity : public GameplayAbilitySystem
	{
		GDCLASS(GAS_Entity, GameplayAbilitySystem)

	protected:
		static void _bind_methods();

	public:
		GAS_Entity();
		~GAS_Entity();

		void Init();

		EntityID GetID() const { return m_ID; }
		void SetID(EntityID id);

		AttributeContainer* GetAttributeContainer() const { return m_AttrContainer; }
		void SetAttributeContainer(AttributeContainer* node) { m_AttrContainer = node; };
		godot::NodePath GetAttributeContainerNodePath() const { return attrContainerNodePath; }
		void SetAttributeContainerNodePath(godot::NodePath path);

		TagContainer* GetTagContainer() const { return m_TagContainer; }
		void SetTagContainer(TagContainer* node) { m_TagContainer = node; };
		godot::NodePath GetTagContainerNodePath() const { return tagContainerNodePath; }
		void SetTagContainerNodePath(godot::NodePath path);

		void AddEffect(const godot::Ref<EffectData> gdEffect);

		bool HandleTags(const godot::Ref<sm::EffectData>& gdEffect);
		void RemoveTags(const BitSet<MAX_TAGS> tags);

	private:
		void OnEnterTree() override;
		void OnExitTree() override;
		void OnReady() override;

	public:
		godot::NodePath attrContainerNodePath = "";
		godot::NodePath tagContainerNodePath = "";

	private:
		AttributeContainer* m_AttrContainer = nullptr;
		TagContainer* m_TagContainer = nullptr;

		WorldBound m_WorldBound;

		EntityID m_ID = 0;
	};
}