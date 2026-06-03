#pragma once
#include "core/TagRegistry.h"
#include "godot/gdTagData.h"
#include "godot/gdGameplayAbilitySystemNode.h"
#include "godot/gdGASEntity.h"
#include "internal/smUID.h"

#include <unordered_set>
#include <godot_cpp/classes/ref.hpp>

namespace sm
{
	class EffectSystem;
	class TagContainer;
	class TagSystem;

	class GAS_World : public GameplayAbilitySystem
	{
		GDCLASS(GAS_World, GameplayAbilitySystem)

	protected:
		static void _bind_methods();

	private:
		GAS_World();
		GAS_World(const GAS_World&) = delete;
		~GAS_World() = default;

	public:

#pragma region Godot
		int GetEntitiesCount() const { return m_EntitiesRegistry.size() - 1; };

		bool GetEffectsAvailability() const { return enableEffects; };
		void SetEffectsAvailability(bool value);

		bool GetAbilitiesAvailability() const { return enableAbilities; };
		void SetAbilitiesAvailability(bool value);

		EffectSystem* GetEffectSystem() const { return m_EffectsSystem.get(); };
		GAS_Entity* GetEntity(EntityID id);

		// Tags
		godot::TypedArray<godot::Node> AllTags(const godot::TypedArray<TagData> tags);

		godot::TypedArray<godot::Node> AnyTags(const godot::TypedArray<TagData> tags);

		godot::TypedArray<godot::Node> NoneTags(const godot::TypedArray<TagData> tags);

#pragma endregion Godot

		EntityID RegisterEntity(GAS_Entity* entity);
		void UnregisterEntity(GAS_Entity* entity);

		void RegisterTagContainer(TagContainer* container);
		void UnregisterTagContainer(TagContainer* container);

	private:
		void OnEnterTree() override;
		void InitTagSystem(godot::Node* globalRoot);
		void OnExitTree() override;
		void OnReady() override;
		void OnProcess() override;

		void _DeferredUpdate();

	public:
		bool enableEffects = true;
		bool enableAbilities = true;

	private:
		DumbUID m_EntityUIDs;

		std::unique_ptr<EffectSystem> m_EffectsSystem;

		// ID: 0 = invalid entity
		std::unordered_map<EntityID, GAS_Entity*> m_EntitiesRegistry;
		std::unordered_set<GAS_Entity*> m_Entities;

		TagSystem* m_TagSystem;
		TagRegistry* m_TagRegistry;
		//godot::Ref<TagData> m_TagRegistry;
	};
}
