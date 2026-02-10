#pragma once
#include "Types.h"
#include "gdGameplayAbilitySystemResource.h"

namespace sm
{
	class ModifierData : public GameplayAbilitySystemResource
	{
		GDCLASS(ModifierData, GameplayAbilitySystemResource)

	protected:
		static void _bind_methods();

	public:
		enum OperationType : uint8_t
		{
			Add = 0,		// Add to CurrentValue
			Multiply,		// Multiply to CurrentValue
			PercentAdd,		// Add x% of Base to CurrentValue
			PercentStack,	// Multiply x% of CurrentValue to CurrentValue
			Override,		// Ignore all modifiers and substitute CurrentValue
		};

		ModifierData::OperationType GetOperationType()  const { return m_Operation; };
		void SetOperationType(ModifierData::OperationType t) { m_Operation = t; };
		AttributeID GetTargetID() const { return m_TargetID; };
		void SetTargetID(AttributeID id) { m_TargetID = id; };
		EffectID GetSourceID() const { return m_SourceID; };
		void SetSourceID(EffectID id) { m_SourceID = id; };
		float GetValue() const { return m_Value; };
		void SetValue(float v) { m_Value = v; };

	private:
		OperationType m_Operation;
		AttributeID m_TargetID;
		EffectID m_SourceID;
		float m_Value;
	};
}

VARIANT_ENUM_CAST(sm::ModifierData::OperationType);
