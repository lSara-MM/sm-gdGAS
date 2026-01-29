#pragma once
#include "GameplayModifier.h"
#include "Types.h"
#include "UID.h"

#include <optional>

namespace sm
{
	class ModifierData;
}

namespace sm
{
	class GameplayAttribute
	{
	private:

		using ModifierPtr = std::unique_ptr<GameplayModifier>;
		static constexpr size_t OperationTypeCount = static_cast<size_t>(sm::GameplayModifier::OperationType::Max);

		using ModifierBuckets = std::array<std::vector<ModifierPtr>, OperationTypeCount>;

	public:
		GameplayAttribute(AttributeID id, float base = 0.0f, float min = 0.0f, float max = FLT_MAX) :
			m_UID(id), m_dirty(true), m_BaseValue(base), m_CurrentValue(base), m_MinValue(min), m_MaxValue(max)
		{

		}

		GameplayAttribute(const GameplayAttribute&) = delete;
		GameplayAttribute& operator=(const GameplayAttribute&) = delete;
		GameplayAttribute(GameplayAttribute&&) = default;
		GameplayAttribute& operator=(GameplayAttribute&&) = default;

		AttributeID GetUID() const { return m_UID; }

		float GetBase() const { return m_BaseValue; }
		void SetBase(float newValue);
		float GetCurrent() const { return m_CurrentValue; }

		float GetMin() const { return m_MinValue; }
		float GetMax() const { return m_MaxValue; }

		bool IsMin() const { return m_CurrentValue <= m_MinValue; }
		bool IsMaxed() const { return m_CurrentValue >= m_MaxValue; }

		GameplayModifier* FindModifier(const godot::Ref<ModifierData>& mod);
		std::optional<size_t> FindModifierIndex(const godot::Ref<ModifierData>& mod) const;
		size_t AddModifier(const godot::Ref<ModifierData>& mod);
		void RemoveModifier(const godot::Ref<ModifierData>& mod);
		void Reset();

	private:
		void Calculate();

	private:
		AttributeID m_UID;
		UID m_ModifiersUID;

		float m_BaseValue;
		float m_CurrentValue;
		float m_MinValue;
		float m_MaxValue;

		bool m_dirty;

		ModifierBuckets m_Modifiers;
	};
}