#include "JSCUtilities.h"
#include "Adapters/JSFunctionCallbackAdapter.h"
#include "Cache/JSCCache.h"
#include "Core/JNIUtilities.h"

#include <JavaScriptCore/JavaScript.h>

#include <vector>

using namespace Luminescence;
using namespace Luminescence::JavaScript;

namespace
{
    /*-----------------------*/
    /* -- Utility Methods -- */
    /*-----------------------*/

    jobject WrapResult(JNIEnv* Environment, jlong ContextHandle, JSContextRef Context, JSValueRef Value)
    {
        if (!Value)
            return nullptr;

        if (JSValueIsArray(Context, Value))
            return Environment->NewObject(CJSArrayCache::ClassRef, CJSArrayCache::Constructor, ContextHandle,
                                          reinterpret_cast<jlong>(Value));

        if (JSValueIsObject(Context, Value))
            return Environment->NewObject(CJSObjectCache::ClassRef, CJSObjectCache::Constructor, ContextHandle,
                                          reinterpret_cast<jlong>(Value));

        return Environment->NewObject(CJSValueCache::ClassRef, CJSValueCache::Constructor, ContextHandle,
                                      reinterpret_cast<jlong>(Value));
    }

    /*-----------------------*/
    /*-----------------------*/
    /*-----------------------*/

    /*
    * Class:     me_ayydxn_luminescence_javascript_JSFunction_NativeMethods
    * Method:    nJSFunctionCreate
    * Signature: (JLjava/lang/String;Lme/ayydxn/luminescence/javascript/JSFunction/JSCallback;)Lme/ayydxn/luminescence/javascript/JSFunction;
    */
    jobject JNICALL JSFunctionCreate_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jstring Name, jobject Callback)
    {
        const auto Context = ToContextRef(ContextHandle);

        // Adapter holds a global ref to Callback; JSC finalizer owns the adapter's lifetime.
        auto* Adapter = new CJSFunctionCallbackAdapter(Environment, Callback);

        JSObjectRef FunctionObject = JSObjectMake(Context, CJSFunctionCallbackAdapter::GetJavaFunctionClass(), Adapter);
        if (!FunctionObject)
        {
            delete Adapter;
            
            Environment->ThrowNew(CJSExceptionCache::ClassRef, "Failed to create JS function object");
            
            return nullptr;
        }

        // Set the .name property so JS stack traces show a useful name
        if (Name)
        {
            const JSStringRef NameStr = JavaStringToJSString(Environment, Name);
            const JSStringRef NameKey = JSStringCreateWithUTF8CString("name");

            JSObjectSetProperty(Context, FunctionObject, NameKey, JSValueMakeString(Context, NameStr),
                kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete, nullptr);

            JSStringRelease(NameStr);
            JSStringRelease(NameKey);
        }

        return Environment->NewObject(CJSFunctionCache::ClassRef, CJSFunctionCache::Constructor, ContextHandle, reinterpret_cast<jlong>(FunctionObject));
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSFunction_NativeMethods
     * Method:    nJSFunctionCall
     * Signature: (JJJ[J)Lme/ayydxn/luminescence/javascript/JSValue;
     */
    jobject JNICALL JSFunctionCall_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jlong FunctionHandle, jlong ThisHandle, jlongArray ArgumentHandles)
    {
        const auto Context  = ToContextRef(ContextHandle);
        const auto Function = ToObjectRef(FunctionHandle);
        const auto This = ThisHandle ? ToObjectRef(ThisHandle) : JSContextGetGlobalObject(Context);

        const jsize ArgumentCount = ArgumentHandles ? Environment->GetArrayLength(ArgumentHandles) : 0;
        std::vector<JSValueRef> Arguments(ArgumentCount);

        if (ArgumentCount > 0)
        {
            jlong* Elements = Environment->GetLongArrayElements(ArgumentHandles, nullptr);
            for (jsize i = 0; i < ArgumentCount; ++i)
                Arguments[i] = ToValueRef(Elements[i]);
            
            Environment->ReleaseLongArrayElements(ArgumentHandles, Elements, JNI_ABORT);
        }

        JSValueRef Exception = nullptr;
        const JSValueRef Result = JSObjectCallAsFunction(Context, Function, This, static_cast<size_t>(ArgumentCount), Arguments.empty() ? nullptr : Arguments.data(),
            &Exception);

        if (ThrowIfJSException(Environment, Context, Exception))
            return nullptr;

        return WrapResult(Environment, ContextHandle, Context, Result);
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSFunction_NativeMethods
     * Method:    nJSFunctionCallAsConstructor
     * Signature: (JJ[J)Lme/ayydxn/luminescence/javascript/JSObject;
     */
    jobject JNICALL JSFunctionCallAsConstructor_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jlong FunctionHandle, jlongArray ArgumentHandles)
    {
        const auto Context  = ToContextRef(ContextHandle);
        const auto Function = ToObjectRef(FunctionHandle);

        const jsize ArgumentCount = ArgumentHandles ? Environment->GetArrayLength(ArgumentHandles) : 0;
        std::vector<JSValueRef> Arguments(ArgumentCount);

        if (ArgumentCount > 0)
        {
            jlong* Elements = Environment->GetLongArrayElements(ArgumentHandles, nullptr);
            for (jsize i = 0; i < ArgumentCount; ++i)
                Arguments[i] = ToValueRef(Elements[i]);
            
            Environment->ReleaseLongArrayElements(ArgumentHandles, Elements, JNI_ABORT);
        }

        JSValueRef Exception = nullptr;
        JSObjectRef Result = JSObjectCallAsConstructor(Context, Function, static_cast<size_t>(ArgumentCount), Arguments.empty() ? nullptr : Arguments.data(), &Exception);

        if (ThrowIfJSException(Environment, Context, Exception))
            return nullptr;

        if (!Result)
            return nullptr;

        return Environment->NewObject(CJSObjectCache::ClassRef, CJSObjectCache::Constructor, ContextHandle, reinterpret_cast<jlong>(Result));
    }
}

static constexpr JNINativeMethod JSFunctionMethods[] = {
    JNI_METHOD("nJSFunctionCreate", "(JLjava/lang/String;Lme/ayydxn/luminescence/javascript/JSFunction$JSCallback;)Lme/ayydxn/luminescence/javascript/JSFunction;",
        JSFunctionCreate_Native),
    JNI_METHOD("nJSFunctionCall", "(JJJ[J)Lme/ayydxn/luminescence/javascript/JSValue;", JSFunctionCall_Native),
    JNI_METHOD("nJSFunctionCallAsConstructor", "(JJ[J)Lme/ayydxn/luminescence/javascript/JSObject;", JSFunctionCallAsConstructor_Native),
};

bool Luminescence::RegisterJSFunctionMethods(JNIEnv* Environment)
{
    return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/javascript/JSFunction$NativeMethods", JNI_METHODS_AND_COUNT(JSFunctionMethods));
}
