#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "gdExample.h"
#include "godot/ui/gdTagRegistryEditor.h"
#include "godot/gdAbilityData.h"
#include "godot/gdAbilityContainer.h"
#include "godot/gdAttributeContainer.h"
#include "godot/gdAttributeData.h"
#include "godot/gdAttributeSetData.h"
#include "godot/gdEffectData.h"
#include "godot/gdGameplayAbilitySystemNode.h"
#include "godot/gdGameplayAbilitySystemResource.h"
#include "godot/gdGASEntity.h"
#include "godot/gdGASWorld.h"
#include "godot/gdTagContainer.h"
#include "godot/gdTagData.h"
#include "godot/gdTagRegistry.h"

using namespace godot;

void initialize_gdextabilities_plugin(ModuleInitializationLevel p_level)
{
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		ClassDB::register_class<GDExample>();
		ClassDB::register_abstract_class<sm::GameplayAbilitySystem>();
		ClassDB::register_abstract_class<sm::GameplayAbilitySystemResource>();

		ClassDB::register_class<sm::GAS_World>();
		ClassDB::register_class<sm::GAS_Entity>();

		ClassDB::register_class<sm::AbilityData>();
		ClassDB::register_class<sm::AttributeData>();
		ClassDB::register_class<sm::AttributeSetData>();
		ClassDB::register_class<sm::EffectData>();
		ClassDB::register_class<sm::ModifierData>();
		ClassDB::register_class<sm::TagData>();
		//ClassDB::register_class<sm::TagRegistryData>();

		ClassDB::register_class<sm::AttributeContainer>();
		ClassDB::register_class<sm::AbilityContainer>();
		ClassDB::register_class<sm::TagContainer>();
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
	{
		ClassDB::register_class<sm::TagRegistryEditor>();
		EditorPlugins::add_by_type<sm::TagRegistryEditor>();
	}
#endif //TOOLS_ENABLED
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