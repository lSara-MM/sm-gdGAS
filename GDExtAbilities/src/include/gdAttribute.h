#pragma once
#include "Types.h"
#include "GameplayAttribute.h"

#include <classes/ref_counted.hpp>

namespace sm
{
	class Attribute : public godot::RefCounted
	{
		GDCLASS(Attribute, godot::RefCounted)

	protected:
		static void _bind_methods();

	public:
		Attribute();
		~Attribute();

		static godot::Ref<sm::Attribute> Create(float base, AttributeID n);
		float GetBaseValue() { return m_Attribute->GetBase(); }
		void SetBaseValue(float value) { m_Attribute->SetBase(value); }
		AttributeID GetUID() { return m_Attribute->GetUID(); }

	private:
		sm::GameplayAttribute* m_Attribute;
	};
}