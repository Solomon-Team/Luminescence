#include "JSCUtilities.h"
#include "Core/JNIUtilities.h"

#include <JavaScriptCore/JavaScript.h>

using namespace Luminescence::JavaScript;

namespace
{
    /*
    * Class:     me_ayydxn_luminescence_javascript_JSArray_NativeMethods
    * Method:    nJSArrayLength
    * Signature: (JJ)I
    */
    jint JSArrayLength_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jlong ObjectHandle)
    {
        const auto ContextRef = ToContextRef(ContextHandle);
        const JSStringRef PropertyName = JSStringCreateWithUTF8CString("length");
        JSValueRef Exception = nullptr;
        const JSValueRef Result = JSObjectGetProperty(ContextRef, ToObjectRef(ObjectHandle), PropertyName, &Exception);
        
        JSStringRelease(PropertyName);
        
        if (ThrowIfJSException(Environment, ContextRef, Exception))
            return 0;
        
        return static_cast<jint>(JSValueToNumber(ContextRef, Result, nullptr));
    }
}

static constexpr JNINativeMethod JSArrayMethods[] = {
    JNI_METHOD("nJSArrayLength", "(JJ)I", JSArrayLength_Native),
};

bool Luminescence::RegisterJSArrayMethods(JNIEnv* Environment)
{
    return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/javascript/JSArray$NativeMethods", JNI_METHODS_AND_COUNT(JSArrayMethods));
}
