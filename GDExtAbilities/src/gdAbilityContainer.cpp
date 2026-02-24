#include "gdAbilityContainer.h"

void sm::AbilityContainer::SetAbilities(const godot::TypedArray<sm::AbilityData>& ability)
{
	m_gdAbilities = ability;
}
