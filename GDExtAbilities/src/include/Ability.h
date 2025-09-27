#pragma once
#include "Attribute.h"
#include "AttributeSet.h"
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>

namespace sm
{
	class Ability : public godot::Resource
	{
		GDCLASS(Ability, godot::Resource)

	private:

	protected:
		static void _bind_methods();

	public:
		Ability();
		~Ability();
		
		void Activate();
		void Deactivate();
		bool CanBeActivated() const;

		bool GetActive() const { return m_IsActive; }
		void SetActive(const bool active) { m_IsActive = active; }

		godot::Ref<sm::AttributeSet> GetAttributeSet() const { return m_Attributes; }
		void SetAttributeSet(const godot::Ref<sm::AttributeSet>& attributes ) { m_Attributes = attributes; }

	public:
		bool m_IsActive = true;
		godot::Ref<sm::AttributeSet> m_Attributes;
	};
}