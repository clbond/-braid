// Automatically generated from /Users/bond/braid/webkit/Source/JavaScriptCore/runtime/SymbolConstructor.cpp using /Users/bond/braid/webkit/Source/JavaScriptCore/create_hash_table. DO NOT EDIT!

#include "Lookup.h"

namespace JSC {

static const struct CompactHashIndex symbolConstructorTableIndex[4] = {
    { 0, -1 },
    { -1, -1 },
    { -1, -1 },
    { 1, -1 },
};

static const struct HashTableValue symbolConstructorTableValues[2] = {
   { "for", DontEnum|Function, NoIntrinsic, { (intptr_t)static_cast<NativeFunction>(symbolConstructorFor), (intptr_t)(1) } },
   { "keyFor", DontEnum|Function, NoIntrinsic, { (intptr_t)static_cast<NativeFunction>(symbolConstructorKeyFor), (intptr_t)(1) } },
};

static const struct HashTable symbolConstructorTable =
    { 2, 3, false, symbolConstructorTableValues, symbolConstructorTableIndex };

} // namespace JSC
