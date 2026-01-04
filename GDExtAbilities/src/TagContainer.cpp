#include "TagContainer.h"

#include "TagRegistry.h"

#include <variant\utility_functions.hpp>

sm::TagContainer::TagContainer()
{}

sm::TagContainer::~TagContainer()
{}

void sm::TagContainer::_bind_methods()
{}

void sm::TagContainer::_ready()
{
	godot::UtilityFunctions::print("hola");
	TagRegistry& instance = TagRegistry::GetInstance();

	instance.RegisterTag("hola.tag");
	instance.RegisterTag("hola.tag.aaa");
	instance.RegisterTag("hola.tag.aaa");
	instance.RegisterTag("hola.tag.bbb");
	instance.RegisterTag("hola.tag.hh");
	instance.RegisterTag("hola.45aa.hh");
	instance.RegisterTag("hola.tag.ccc");

	ERR_PRINT("tag 3 is: " + godot::String(instance.GetTagName(3)));
}

void sm::TagContainer::Test(godot::StringName name)
{
	TagRegistry& instance = TagRegistry::GetInstance();
	std::cout << "Ping desde C++" << std::endl;
}
