// This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details
#include "luacode.h"

#include "Luau/Compiler.h"
#include "Luau/BytecodeBuilder.h"

#include <string.h>

class RobloxBytecodeEncoder : public Luau::BytecodeEncoder
{
    uint8_t encodeOp(uint8_t op)
    {
        return op * 227;
    }
};

char* luau_compile(const char* source, size_t size, lua_CompileOptions* options, size_t* outsize)
{
    LUAU_ASSERT(outsize);

    Luau::CompileOptions opts;
    RobloxBytecodeEncoder encoder;

    if (options)
    {
        static_assert(sizeof(lua_CompileOptions) == sizeof(Luau::CompileOptions), "C and C++ interface must match");
        memcpy(static_cast<void*>(&opts), options, sizeof(opts));
    }

    std::string result = compile(std::string(source, size), opts, {}, &encoder);

    char* copy = static_cast<char*>(malloc(result.size()));
    if (!copy)
        return nullptr;

    memcpy(copy, result.data(), result.size());
    *outsize = result.size();
    return copy;
}
