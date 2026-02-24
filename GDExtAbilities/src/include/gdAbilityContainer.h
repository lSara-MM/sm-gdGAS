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

		// On any notification. (similar to OnCollision from Unity)
		void _notification(int notification);

		godot::TypedArray<sm::AbilityData> GetAbilities() const { return m_gdAbilities; };
		void SetAbilities(const godot::TypedArray<sm::AbilityData>& ability);

#pragma region Godot public 

#pragma endregion

	private:
		AbilityContainer();
		~AbilityContainer() = default;

		void OnEnterTree();
		void OnExitTree();

		// When node container is loaded, get editor changes and apply them to the C++ AttributeData Set
		void OnReady();

	private:
		godot::TypedArray<AbilityData> m_gdAbilities;
		TagContainer* m_TagContainer;

		//std::unordered_map<AttributeID, GameplayAbility*> m_AttributesByName;
	};
}