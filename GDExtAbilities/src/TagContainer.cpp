#include "TagContainer.h"

#include "TagRegistry.h"

#include <variant/utility_functions.hpp>
#include <classes/object.hpp>

sm::TagContainer::TagContainer()
{}

sm::TagContainer::~TagContainer()
{}

void sm::TagContainer::_bind_methods()
{}

void sm::TagContainer::_ready()
{
	/*godot::UtilityFunctions::print("hola");
	TagRegistry& instance = TagRegistry::GetInstance();

	instance.RegisterTag("hola.tag");
	instance.RegisterTag("hola.tag.aaa");
	instance.RegisterTag("hola.tag.aaa");
	instance.RegisterTag("hola.tag.ccc");
	instance.RegisterTag("hola.TAG.aaa");
	instance.RegisterTag("hola.GA.bbb");
	instance.RegisterTag("hola.tag.hh");
	instance.RegisterTag("hola.45aa.hh");
	instance.RegisterTag("hola.33daa.hh");

	ERR_PRINT("PARENT");
	ERR_PRINT(instance.GetParent("bbb"));
	
	ERR_PRINT("TAGS");
	ERR_PRINT(instance.GetTag("bbb"));
	ERR_PRINT(instance.GetTag("TAG"));
	ERR_PRINT(instance.GetTag("TAGa"));

	ERR_PRINT("CHILDREN");
	godot::TypedArray<godot::StringName> arr = instance.GetChildren("hola");
	for (size_t i = 0; i < arr.size(); i++)
	{
		ERR_PRINT(arr[i]);
	}

	ERR_PRINT("ASCENDANTS");
	godot::TypedArray<godot::StringName> arr2 = instance.GetAscendants("ccc");
	for (size_t i = 0; i < arr2.size(); i++)
	{
		ERR_PRINT(arr2[i]);
	}
	godot::TypedArray<godot::StringName> arr5 = instance.GetAscendants("ccc");
	for (size_t i = 0; i < arr5.size(); i++)
	{
		ERR_PRINT(arr5[i]);
	}

	ERR_PRINT("DESCENDANTS");
	godot::TypedArray<godot::StringName> arr3 = instance.GetDescendants("hola");
	for (size_t i = 0; i < arr3.size(); i++)
	{
		ERR_PRINT(arr3[i]);
	}
	godot::TypedArray<godot::StringName> arr6 = instance.GetDescendants("hola");
	for (size_t i = 0; i < arr6.size(); i++)
	{
		ERR_PRINT(arr6[i]);
	}

	instance.UnregisterTag("hola.tag.hh");
	instance.UnregisterTag("hola.tag.ccc");
	instance.UnregisterTag("hola.tag.ccc");
	instance.UnregisterTag("hola.tag");

	ERR_PRINT("CHILDREN");
	godot::TypedArray<godot::StringName> arr4 = instance.GetChildren("hola");
	for (size_t i = 0; i < arr3.size(); i++)
	{
		ERR_PRINT(arr3[i]);
	}*/
}

void sm::TagContainer::Test(godot::StringName name)
{
	TagRegistry& instance = TagRegistry::GetInstance();
	std::cout << "Ping desde C++" << std::endl;
}
