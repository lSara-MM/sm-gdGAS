#pragma once
#include "GameplayAbilitySystemNode.h"

#include <godot_cpp/classes/ref.hpp>

namespace sm
{
	class AttributeSet;
}

namespace sm
{
	class AttributeContainer : public sm::GameplayAbilitySystem
	{
		GDCLASS(AttributeContainer, sm::GameplayAbilitySystem)

	protected:
		static void _bind_methods();

	public:
		
		godot::Ref<sm::AttributeSet> GetAttributeSet();
		void SetAttributeSet(const godot::Ref<sm::AttributeSet>& attr);

		void AddAttribute(godot::StringName name, float baseValue);

	private:
		AttributeContainer() = default;
		~AttributeContainer() = default;

	private:
		godot::Ref<sm::AttributeSet> m_gdAttributeSet;
	};
}