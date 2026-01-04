#pragma once
#include <godot_cpp/classes/resource.hpp>

namespace sm
{
	class Attribute : public godot::Resource
	{
		GDCLASS(Attribute, godot::Resource)

	protected:
		static void _bind_methods();

	public:
		Attribute();
		~Attribute();
	};

	//class FloatAttribute : public Attribute
	//{
	//	GDCLASS(FloatAttribute, Attribute)

	//protected:
	//	static void _bind_methods();

	//public:
	//	FloatAttribute(float base = 0.0f, float current = NAN);
	//	~FloatAttribute();

	//	void Initialize(float base = 0.0f, float current = NAN);

	//	float GetBaseValue() const { return m_BaseValue; }
	//	void SetBaseValue(float value) { m_BaseValue = value; }

	//	float GetCurrentValue() const { return m_CurrentValue; }
	//	void SetCurrentValue(float value) { m_CurrentValue = value; }

	//public:
	//	float m_BaseValue;
	//	float m_CurrentValue;
	//};
}