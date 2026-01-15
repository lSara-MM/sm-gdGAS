#pragma once
#include "GameplayAttribute.h"

#include <vector>

namespace sm
{
	class GameplayAttributeSet
	{
	public:
		GameplayAttributeSet();
		~GameplayAttributeSet();

	private:
		std::vector<GameplayAttribute> m_Attributes;
	};

	class AttributeSet : public godot::Resource
	{
		GDCLASS(AttributeSet, godot::Resource)

	protected:
		static void _bind_methods();

	public:

		godot::TypedArray<sm::Attribute> GetAttributesSet();
		void SetAttributesSet(const godot::TypedArray<sm::Attribute>& attr);

	private:
		AttributeSet() = default;
		~AttributeSet() = default;

	private:
		godot::TypedArray<sm::Attribute> m_gdAttributes;
	};
}