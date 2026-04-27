#pragma once
#include "godot/gdGameplayAbilitySystemNode.h"
#include "godot/gdAbilityData.h"
#include "godot/GASWorldBound.h"
#include "internal/Types.h"

#include <godot_cpp/variant/node_path.hpp>

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

		godot::TypedArray<sm::AbilityData> GetAbilities() const { return m_gdAbilities; };
		void SetAbilities(const godot::TypedArray<sm::AbilityData>& ability);

		godot::NodePath	GetEntityNodePath() const;
		void SetEntityNodePath(godot::NodePath path);

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