#include "gdAttribute.h"

#include "gdAttributeContainer.h"

sm::Attribute::Attribute()
{

}

sm::Attribute::~Attribute()
{
	RELEASE(m_Attribute);
}

void sm::Attribute::_bind_methods()
{
	//godot::ClassDB::bind_static_method("Attribute", godot::D_METHOD("create", "base_value", "name"), &Create);
}

//godot::Ref<sm::Attribute> sm::Attribute::Create(AttributeID n, float base)
//{
//	godot::Ref<sm::Attribute> attr;
//	attr.instantiate();
//	attr->SetBaseValue(base);
//	
//	return attr;
//}