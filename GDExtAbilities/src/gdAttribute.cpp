#include "gdAttribute.h"

sm::Attribute::Attribute()
{

}

sm::Attribute::~Attribute()
{

}

void sm::Attribute::_bind_methods()
{
	godot::ClassDB::bind_static_method("Attribute", godot::D_METHOD("create", "base_value", "name"), &Create);
}

godot::Ref<sm::Attribute> sm::Attribute::Create(float base, AttributeID n)
{
	godot::Ref<sm::Attribute> attr;
	attr.instantiate();
	attr->SetBaseValue(base);
	
	return attr;
}