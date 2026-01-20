#pragma once
#include "GameplayAbilitySystemNode.h"
#include "GameplayAttribute.h"
#include "gdAttribute.h"

#include <godot_cpp/classes/ref.hpp>
#include <memory>

namespace sm
{
	class AttributeSet;
	class GameplayAttributeSet;
}

namespace sm
{
	class AttributeContainer : public sm::GameplayAbilitySystem
	{
		GDCLASS(AttributeContainer, sm::GameplayAbilitySystem)

	protected:
		static void _bind_methods();

	public:

		// On any notification. (similar to OnCollision from Unity)
		void _notification(int notification);

#pragma region Godot public 

		godot::Ref<sm::AttributeSet> GetAttributeSet();
		void SetAttributeSet(const godot::Ref<sm::AttributeSet>& attr);

		/*void AddAttribute(godot::StringName name, float m_BaseValue);
		void AddAttribute(const godot::Ref<sm::Attribute>& attr);*/

		float GetAttributeID(godot::StringName name);

#pragma endregion 

	private:
		AttributeContainer();
		~AttributeContainer();

		uint32 _GenerateUID() { return m_NextUID++; }

	private:
		uint32 m_NextUID = 0;
		godot::Ref<sm::AttributeSet> m_gdAttributeSet;
		std::unique_ptr<sm::GameplayAttributeSet> m_AttributeSetPtr;
	};
}