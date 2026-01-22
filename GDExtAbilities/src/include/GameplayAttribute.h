#pragma once
#include "GameplayModifier.h"
#include "Types.h"

namespace sm
{
	class GameplayAttribute
	{
	public:
		GameplayAttribute(AttributeID id = 0, float base = 0.0f, float min = 0.0f, float max = FLT_MAX) :
			m_UID(id), m_dirty(true), m_BaseValue(base), m_CurrentValue(base), m_MinValue(min), m_MaxValue(max)
		{

		}

		AttributeID GetUID() const { return m_UID; }

		float GetBase() const { return m_BaseValue; }
		float GetCurrent() const { return m_CurrentValue; }

		float GetMin() const { return m_MinValue; }
		float GetMax() const { return m_MaxValue; }
		
		bool IsMin() const { return m_CurrentValue <= m_MinValue; }
		bool IsMaxed() const { return m_CurrentValue >= m_MaxValue; }

		void AddModifier(const sm::GameplayModifier& mod);
		void Reset();

	private:
		void Calculate();

	private:
		AttributeID m_UID;

		float m_BaseValue;
		float m_CurrentValue;
		float m_MinValue;
		float m_MaxValue;
		
		bool m_dirty;

		std::unordered_map<ModifierID, std::shared_ptr<sm::GameplayModifier>> m_Modifiers;
	};
}