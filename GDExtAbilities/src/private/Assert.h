#pragma once
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#ifdef TOOLS_DEBUG_VS
#   define SM_ASSERT(Expr, Msg, ...) \
    __SM_Assert(#Expr, Expr, __FILE__, __LINE__, Msg, __VA_ARGS__)
#else
#   define SM_ASSERT(Expr, Msg, ...);
#endif

inline void __SM_Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg, ...)
{
    if (!expr)
    {
        char buffer[1024];

        va_list args;
        va_start(args, msg);
        vsnprintf(buffer, sizeof(buffer), msg, args);
        va_end(args);

        std::cerr << "Assert:\t" << buffer << "\n"
            << "Expected:\t" << expr_str << "\n"
            << "Source:\t\t" << file << ", line " << line << "\n";
        abort();
    }
}