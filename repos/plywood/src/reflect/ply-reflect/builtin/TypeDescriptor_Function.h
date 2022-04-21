/*------------------------------------
  ///\  Plywood C++ Framework
  \\\/  https://plywood.arc80.com/
------------------------------------*/
#pragma once
#include <ply-reflect/Core.h>
#include <ply-reflect/TypeDescriptor_Def.h>

#if PLY_WITH_METHOD_TABLES
#include <ply-reflect/methods/BaseInterpreter.h>
#endif

namespace ply {

PLY_DLL_ENTRY extern TypeKey TypeKey_Function;

PLY_DLL_ENTRY NativeBindings& getNativeBindings_Function();

struct TypeDescriptor_Function : TypeDescriptor {
    PLY_DLL_ENTRY static TypeKey* typeKey;
    Array<TypeDescriptor*> paramTypes;

    PLY_INLINE TypeDescriptor_Function()
        : TypeDescriptor{&TypeKey_Function, (void**) nullptr,
                         getNativeBindings_Function() PLY_METHOD_TABLES_ONLY(, {})} {
    }
};

namespace details {

#if PLY_WITH_METHOD_TABLES

template <typename... Params>
struct FunctionCallThunk {};
template <typename Param0>
struct FunctionCallThunk<Param0> {
    static PLY_NO_INLINE MethodResult call(BaseInterpreter* interp, const AnyObject& callee) {
        using FunctionType = MethodResult(BaseInterpreter*, Param0);
        PLY_ASSERT(callee.type->isEquivalentTo(getTypeDescriptor<FunctionType>()));
        const AnyObject& arg = interp->localVariableStorage.items.tail();
        PLY_ASSERT(arg.type->isEquivalentTo(getTypeDescriptor<Param0>()));
        return reinterpret_cast<FunctionType*>(callee.data)(interp, *(Param0*) arg.data);
    }
};
template <>
struct FunctionCallThunk<const AnyObject&> {
    static PLY_NO_INLINE MethodResult call(BaseInterpreter* interp, const AnyObject& callee) {
        using FunctionType = MethodResult(BaseInterpreter*, const AnyObject&);
        PLY_ASSERT(callee.type->isEquivalentTo(getTypeDescriptor<FunctionType>()));
        const AnyObject& arg = interp->localVariableStorage.items.tail();
        return reinterpret_cast<FunctionType*>(callee.data)(interp, arg);
    }
};

#endif // PLY_WITH_METHOD_TABLES

template <typename... Params>
struct ParameterAdder {
    static PLY_INLINE void add(TypeDescriptor_Function*) {
    }
};
template <typename Param, typename... Rest>
struct ParameterAdder<Param, Rest...> {
    static PLY_INLINE void add(TypeDescriptor_Function* functionType) {
        functionType->paramTypes.append(getTypeDescriptor<Param>());
        ParameterAdder<Rest...>::add(functionType);
    }
};
template <typename... Rest>
struct ParameterAdder<const AnyObject&, Rest...> {
    static PLY_INLINE void add(TypeDescriptor_Function* functionType) {
        functionType->paramTypes.append(nullptr);
        ParameterAdder<Rest...>::add(functionType);
    }
};

} // namespace details

// We can only create a TypeDescriptor for a function if it accepts ObjectStack* as its first
// parameter and returns an AnyObject.
template <typename... Params>
struct TypeDescriptorSpecializer<MethodResult(BaseInterpreter*, Params...)> {
    static PLY_INLINE TypeDescriptor_Function initType() {
        TypeDescriptor_Function functionType;
        PLY_METHOD_TABLES_ONLY(functionType.methods.call =
                                   details::FunctionCallThunk<Params...>::call;)
        functionType.paramTypes.reserve(sizeof...(Params));
        details::ParameterAdder<Params...>::add(&functionType);
        return functionType;
    }

    static PLY_NO_INLINE TypeDescriptor_Function* get() {
        static TypeDescriptor_Function typeDesc = initType();
        return &typeDesc;
    }
};

} // namespace ply
