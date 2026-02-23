#pragma once
#include "Types.h"
#include "GameplayAttribute.h"

#include <godot_cpp/classes/ref_counted.hpp>

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

		//static godot::Ref<sm::Attribute> Create(AttributeID n, float base);
		float GetBaseValue() { return m_Attribute->GetBase(); }
		void SetBaseValue(float value) { m_Attribute->SetBase(value); }
		AttributeID GetUID() { return m_Attribute->GetUID(); }

		sm::GameplayAttribute* GetAttribute() { return m_Attribute; };

	private:
		sm::GameplayAttribute* m_Attribute;
	};
}