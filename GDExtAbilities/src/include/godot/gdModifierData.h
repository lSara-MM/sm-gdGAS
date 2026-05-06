#pragma once
#include "internal/Types.h"
#include "godot/gdGameplayAbilitySystemResource.h"
#include "core/GameplayModifier.h"

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
			Add = 0,		// Add a flat value to CurrentValue
			Multiply,		// Raw Multiply to CurrentValue
			PercentAdd,		// Add x% of Base to CurrentValue (ex. 20%)
			PercentStack,	// Multiply CurrentValue by a percentage (ex. 20%)
			Override,		// Ignore all modifiers and replace CurrentValue
		};

		ModifierData::OperationType GetOperationType()  const { return m_Operation; };
		ModifierOperationType GetGameplayOperationType()  const { return static_cast<ModifierOperationType>(m_Operation); };
		void SetOperationType(ModifierData::OperationType t) { m_Operation = t; };
		AttributeID GetTargetID() const { return m_TargetID; };
		void SetTargetID(AttributeID id) { m_TargetID = id; };
		EffectID GetSourceID() const { return m_SourceID; };
		void SetSourceID(EffectID id) { m_SourceID = id; };
		float GetValue() const { return m_Value; };
		void SetValue(float v) { m_Value = v; };

	private:
		OperationType m_Operation = ModifierData::OperationType::Add;
		AttributeID m_TargetID;
		EffectID m_SourceID;
		float m_Value;
	};
}

VARIANT_ENUM_CAST(sm::ModifierData::OperationType);
