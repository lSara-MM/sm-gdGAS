#pragma once
#include "GameplayModifier.h"
#include "Types.h"
#include "UID.h"

#include <optional>
#include <queue>

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
		static constexpr size_t OperationTypeCount = static_cast<size_t>(sm::ModifierOperationType::Max);

		using ModifierBuckets = std::array<std::vector<ModifierPtr>, OperationTypeCount>;

	public:
		GameplayAttribute(AttributeID id, float base = 0.0f, float min = 0.0f, float max = FLT_MAX) :
			m_ID(id), m_dirty(true), m_BaseValue(base), m_CurrentValue(base), m_MinValue(min), m_MaxValue(max)
		{

		}

		GameplayAttribute(const GameplayAttribute&) = delete;
		GameplayAttribute& operator=(const GameplayAttribute&) = delete;
		GameplayAttribute(GameplayAttribute&&) = default;
		GameplayAttribute& operator=(GameplayAttribute&&) = default;

		AttributeID GetUID() const { return m_ID; }

		float GetBase() const { return m_BaseValue; }
		void SetBase(float newValue);
		float GetCurrent();

		float GetMin() const { return m_MinValue; }
		float GetMax() const { return m_MaxValue; }

		bool IsMin() const { return m_CurrentValue <= m_MinValue; }
		bool IsMax() const { return m_CurrentValue >= m_MaxValue; }
		bool IsDirty() const { return m_dirty; }

		size_t GetModifiersCount(ModifierOperationType op) const;
		GameplayModifier* FindModifier(const godot::Ref<ModifierData>& mod);
		GameplayModifier* FindModifier(const ModifierHandle& handle);
		std::optional<size_t> FindModifierIndex(const godot::Ref<ModifierData>& mod) const;
		ModifierHandle& AddModifier(const godot::Ref<ModifierData>& mod);
		void RemoveModifier(const godot::Ref<ModifierData>& mod);
		void RemoveModifier(ModifierHandle& handle);

		void AddBaseModifier(const godot::Ref<sm::ModifierData>& mod);

		void ClearModifiers();
		void Reset();

	private:
		void CalculateBase();
		void Calculate();

	private:
		std::queue<GameplayModifier> m_BaseModifiers;
		ModifierBuckets m_Modifiers;

		AttributeID m_ID;
		DumbUID m_ModifiersUIDs;

		float m_BaseValue;
		float m_CurrentValue;
		float m_MinValue;
		float m_MaxValue;

		bool m_dirty;
	};
}