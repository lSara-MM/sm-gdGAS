#include "gdModifier.h"

#include "GameplayModifier.h"

void sm::Modifier::_bind_methods()
{
	BIND_ENUM_CONSTANT(Modifier::OperationType::Add);
	BIND_ENUM_CONSTANT(Modifier::OperationType::Multiply);
	BIND_ENUM_CONSTANT(Modifier::OperationType::Percent);
	BIND_ENUM_CONSTANT(Modifier::OperationType::Override);

}