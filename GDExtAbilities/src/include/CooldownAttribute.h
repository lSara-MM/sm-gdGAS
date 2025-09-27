#pragma once
#include "Attribute.h"

namespace sm
{
	class CooldownAttribute : public Attribute
	{
		GDCLASS(CooldownAttribute, Attribute)

	protected:
		static void _bind_methods();

	public:
		CooldownAttribute(const float cd = 1.0f);
		~CooldownAttribute();

		void _init(const float cd = 1.0f);

		float GetCooldown() const { return m_Cooldown; }
		void SetCooldown(const float cd) { m_Cooldown = cd; }
		float GetCurrentTimer() const { return m_CurrentTimer; }
		void ResetCooldown();

		void Activate();

	public:
		float m_Cooldown;
		float m_CurrentTimer;
	};
}