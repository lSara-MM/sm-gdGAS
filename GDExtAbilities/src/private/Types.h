#pragma once
#include <godot_cpp/classes/ref.hpp>
#include <stdio.h>
#include <string>

#define SM_ASSERT(cond, msg) assert((msg, cond))

#if defined(TOOLS_ENABLED) || defined(DEBUG_ENABLED)
#define DEBUG_MODE
#endif

#if defined(_MSC_VER) && (defined(TOOLS_ENABLED) || defined(DEBUG_ENABLED))
#define TOOLS_DEBUG_VS
#endif // defined(_MSC_VER) && (defined(TOOLS_ENABLED) || defined(DEBUG_ENABLED))

// NOTES: ERR_PRINT("Error. No para ejecucion, no return. Solo escribe consola");
// NOTES: ERR_FAIL_COND_MSG(cond, "Error. Return de la funcion actual.");
// NOTES: UtilityFunctions::push_error("Error. No para ejecucion, no return. Warning en editor");

using uint32 = uint32_t;

inline std::string ToStdString(const godot::StringName& sn)
{
	godot::String gstr = sn;                 // Convert to Godot String
	godot::CharString utf8 = gstr.utf8();    // Convert to UTF-8 buffer
	return std::string(utf8.get_data());     // Copy into std::string
}

inline std::string ToStdString(const godot::String& s)
{
	godot::CharString utf8 = s.utf8();      // Convert to UTF-8 buffer
	return std::string(utf8.get_data());    // Copy into std::string
}
