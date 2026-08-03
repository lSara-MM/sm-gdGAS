#pragma once
#include "godot/GASWorldBound.h"
#include "godot/gdAbilityData.h"
#include "godot/gdGameplayAbility.h"
#include "godot/gdGameplayAbilitySystemNode.h"
#include "internal/Types.h"

#include <godot_cpp/core/gdvirtual.gen.inc>
#include <godot_cpp/variant/node_path.hpp>
#include <unordered_map>

namespace sm
{
	class TagContainer;

	class AbilityContainer : public GameplayAbilitySystem
	{
		GDCLASS(AbilityContainer, GameplayAbilitySystem)

	protected:
		static void _bind_methods();

	public:
#pragma region Godot public 
		godot::TypedArray<AbilityData> GetAbilities() const;
		void SetAbilities(const godot::TypedArray<AbilityData>& ability);

		godot::Ref<GameplayAbility> GetAbilityInstance(const godot::Ref<AbilityData>& ability);

		godot::NodePath	GetEntityNodePath() const;
		bool SetEntityNodePath(godot::NodePath path);

		void AddAbility(const godot::Ref<AbilityData>& ability);

		void RemoveAbility(const godot::Ref<AbilityData>& ability);

		bool Grant(const godot::Ref<AbilityData>& ability);
		bool Revoke(const godot::Ref<AbilityData>& ability);
		void Clear();
		bool Has(const godot::Ref<AbilityData>& ability) const;

		bool IsActive(TagID id) const;
		bool IsOnCooldown(TagID id) const;
		float GetCurrentCooldown(TagID id) const;

		//bool TryActivate(const godot::Ref<AbilityData>& ability);
		bool TryActivate(TagID abilityID);

		bool TryActivateAbilitiesWithTag(godot::PackedInt32Array tags);

		GDVIRTUAL1R(bool, _can_be_granted, godot::Ref<AbilityData>)
			GDVIRTUAL1R(bool, _can_activate, godot::Ref<AbilityData>)
			GDVIRTUAL1R(bool, _on_ability_already_granted, godot::Ref<AbilityData>)
#pragma endregion

			bool InitAbilityScript(const godot::Ref<AbilityData>& ability);

	private:
		AbilityContainer() = default;
		~AbilityContainer() = default;

		void OnReady() override;

	private:
		godot::TypedArray<AbilityData> m_gdAbilities;
		std::unordered_map<TagID, godot::Ref<GameplayAbility>> m_Scripts;

		godot::NodePath m_EntityNodePath = "";
		GAS_Entity* m_Owner = nullptr;
	};
}