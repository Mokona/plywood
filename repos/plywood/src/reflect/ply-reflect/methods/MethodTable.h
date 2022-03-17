/*------------------------------------
  ///\  Plywood C++ Framework
  \\\/  https://plywood.arc80.com/
------------------------------------*/
#pragma once
#include <ply-reflect/Core.h>

#if PLY_WITH_METHOD_TABLES

#include <ply-runtime/io/OutStream.h>
#include <ply-reflect/methods/ObjectStack.h>

namespace ply {

struct AnyObject;

struct MethodTable {
    enum class UnaryOp {
        Negate,
        LogicalNot,
        BitComplement,

        // Container protocol:
        IsEmpty,
        NumItems,

        // List protocol:
        Head,
        Tail,
        Dereference,
        Next,
        Prev,
    };

    enum class BinaryOp {
        Add,
        Subtract,
        Multiply,
        Divide,
        Modulo,
    };

    void (*unaryOp)(ObjectStack* stack, UnaryOp op, const AnyObject& obj) = nullptr;
    void (*binaryOp)(ObjectStack* stack, BinaryOp op, const AnyObject& first,
                     const AnyObject& second) = nullptr;
    void (*propertyLookup)(ObjectStack* stack, const AnyObject& obj,
                           StringView propertyName) = nullptr;
    void (*subscript)(ObjectStack* stack, const AnyObject& obj, u32 index) = nullptr;
    void (*print)(ObjectStack* stack, OutStream* outs, StringView formatSpec,
                  const AnyObject& obj) = nullptr;
    void (*call)(ObjectStack* stack, const AnyObject& obj) = nullptr;

    MethodTable();
};

} // namespace ply

#endif // PLY_WITH_METHOD_TABLES
