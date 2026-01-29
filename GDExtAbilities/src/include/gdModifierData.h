#pragma once
#include "Types.h"

#include <godot_cpp/classes/resource.hpp>

namespace sm
{
	class ModifierData : public godot::Resource
	{
		GDCLASS(ModifierData, godot::Resource)

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

		ModifierData::OperationType GetOperationType() { return m_Operation; };
		void SetOperationType(ModifierData::OperationType t) { m_Operation = t; };
		uint32 GetTargetID() { return m_TargetID; };
		void SetTargetID(uint32 id) { m_TargetID = id; };
		uint32 GetSourceID() { return m_SourceID; };
		void SetSourceID(uint32 id) { m_SourceID = id; };
		float GetValue() { return m_Value; };
		void SetValue(float v) { m_Value = v; };

	private:
		OperationType m_Operation;
		uint32 m_TargetID;
		uint32 m_SourceID;
		float m_Value;
	};
}

VARIANT_ENUM_CAST(sm::ModifierData::OperationType);
