#pragma once
#include "Types.h"

namespace sm
{
	class GameplayAttribute
	{
	public:
		GameplayAttribute(uint32 id = 0, float base = 0.0f, float min = 0.0f, float max = FLT_MAX) :
			UID(id), m_dirty(true), m_BaseValue(base), m_CurrentValue(base), m_MinValue(min), m_MaxValue(max)
		{

		}

		float GetBase() const { return m_BaseValue; }
		float GetCurrent() const { return m_CurrentValue; }

		float GetMin() const { return m_MinValue; }
		float GetMax() const { return m_MaxValue; }
		
		bool IsMin() const { return m_CurrentValue <= m_MinValue; }
		bool IsMaxed() const { return m_CurrentValue >= m_MaxValue; }

		void Calculate();
		void Reset();

	private:
		uint32 UID;

		float m_BaseValue;
		float m_CurrentValue;
		float m_MinValue;
		float m_MaxValue;
		
		bool m_dirty;

		std::vector<GameplayAttribute> m_Modifiers;
	};
}