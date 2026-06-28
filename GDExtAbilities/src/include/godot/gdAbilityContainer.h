#pragma once
#include "godot/gdGameplayAbilitySystemNode.h"
#include "godot/gdAbilityData.h"
#include "godot/GASWorldBound.h"
#include "internal/Types.h"

#include <godot_cpp/variant/node_path.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

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

		godot::TypedArray<AbilityData> GetAbilities() const { return m_gdAbilities; };
		void SetAbilities(const godot::TypedArray<AbilityData>& ability);

		godot::NodePath	GetEntityNodePath() const;
		void SetEntityNodePath(godot::NodePath path);

		bool GrantAbility(godot::Ref<AbilityData> ability);

		GDVIRTUAL1R(bool, _can_be_granted, godot::Ref<AbilityData>)

#pragma endregion

	private:
		AbilityContainer() = default;
		~AbilityContainer() = default;

		void OnReady() override;
		void OnExitTree() override;

	private:
		godot::TypedArray<AbilityData> m_gdAbilities;
		godot::NodePath m_EntityNodePath = "";
		WorldBound m_WorldBound;
	};
}