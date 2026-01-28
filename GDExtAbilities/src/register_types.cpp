#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "gdExample.h"
#include "include/gdAttributeContainer.h"
#include "include/gdAttributeData.h"
#include "include/gdAttributeSetData.h"
#include "include/TagContainer.h"

using namespace godot;

void initialize_gdextabilities_plugin(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        return;
    }

    ClassDB::register_class<GDExample>();
    ClassDB::register_abstract_class<sm::GameplayAbilitySystem>();
    ClassDB::register_class<sm::AttributeData>();
    ClassDB::register_class<sm::AttributeContainer>();
    ClassDB::register_class<sm::AttributeSetData>();
    ClassDB::register_class<sm::ModifierData>();
    ClassDB::register_class<sm::TagContainer>();
}

void uninitialize_gdextabilities_plugin(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        return;
    }
}

extern "C"
{
    auto GDE_EXPORT gdextabilities_plugin_entry(
        GDExtensionInterfaceGetProcAddress p_get_proc_address,
        const GDExtensionClassLibraryPtr p_library,
        GDExtensionInitialization* r_initialization) -> GDExtensionBool
    {
        godot::GDExtensionBinding::InitObject init_obj(
            p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_gdextabilities_plugin);
        init_obj.register_terminator(uninitialize_gdextabilities_plugin);

        init_obj.set_minimum_library_initialization_level(
            MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}