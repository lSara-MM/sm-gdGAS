#pragma once
#include "Types.h"

#include <godot_cpp/classes/resource.hpp>

namespace sm
{
	class Modifier : public godot::Resource
	{
		GDCLASS(Modifier, godot::Resource)

	protected:
		static void _bind_methods();

	public:
		enum OperationType
		{
			Add = 0,
			Multiply,
			Percent,
			Override
		};

		OperationType operation;
		uint32 targetID;
		uint32 sourceID;
		float value;
	};
}

VARIANT_ENUM_CAST(sm::Modifier::OperationType);