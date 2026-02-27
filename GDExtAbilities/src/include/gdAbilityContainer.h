#pragma once
#include "gdGameplayAbilitySystemNode.h"
#include "gdAbilityData.h"
#include "Types.h"

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

#pragma endregion

	private:
		AbilityContainer();
		~AbilityContainer() = default;

	private:
		godot::TypedArray<AbilityData> m_gdAbilities;
		TagContainer* m_TagContainer;
	};
}