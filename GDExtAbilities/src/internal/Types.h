#pragma once
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <stdio.h>
#include <string>

// NOTES: ERR_PRINT("Error. No para ejecucion, no return. Solo escribe consola");
// NOTES: ERR_FAIL_COND_MSG(cond, "Error. Return de la funcion actual.");
// NOTES: UtilityFunctions::push_error("Error. No para ejecucion, no return. Warning en editor");

// Deletes a buffer
#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;

using AttributeID = godot::StringName;
using AttributeSetID = uint32;
using EntityID = uint32;
using AbilityID = godot::StringName;
using EffectID = godot::StringName;
using EffectInstanceID = uint32;
using ModifierID = uint16;
using TagID = uint32;

inline constexpr size_t MAX_TAGS = 1024;

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