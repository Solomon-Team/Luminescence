#include "JSCUtilities.h"
#include "Cache/JSCCache.h"
#include "Core/JNIUtilities.h"

#include <JavaScriptCore/JavaScript.h>

using namespace Luminescence::JavaScript;

namespace
{
    /*
     * Class:     me_ayydxn_luminescence_javascript_JSValue_NativeMethods
     * Method:    nJSValueGetType
     * Signature: (JJ)Lme/ayydxn/luminescence/javascript/JSType;
     */
    jobject JNICALL JSValueGetType_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jlong ValueHandle)
    {
        const int ValueType = JSValueGetType(ToContextRef(ContextHandle), ToValueRef(ValueHandle));
        
        return Environment->CallStaticObjectMethod(CJSTypeCache::ClassRef, CJSTypeCache::FromNativeMethodID, ValueType);
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSValue_NativeMethods
     * Method:    nJSValueIsObject
     * Signature: (JJ)Z
     */
    jboolean JNICALL JSValueIsObject_Native(JNIEnv*, jclass, jlong ContextHandle, jlong ValueHandle)
    {
        return JSValueIsObject(ToContextRef(ContextHandle), ToValueRef(ValueHandle));
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSValue_NativeMethods
     * Method:    nJSValueIsArray
     * Signature: (JJ)Z
     */
    jboolean JNICALL JSValueIsArray_Native(JNIEnv*, jclass, jlong ContextHandle, jlong ValueHandle)
    {
        return JSValueIsArray(ToContextRef(ContextHandle), ToValueRef(ValueHandle));
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSValue_NativeMethods
     * Method:    nJSValueIsFunction
     * Signature: (JJ)Z
     */
    jboolean JNICALL JSValueIsFunction_Native(JNIEnv*, jclass, jlong ContextHandle, jlong ValueHandle)
    {
        const auto ContextRef = ToContextRef(ContextHandle);

        return JSObjectIsFunction(ContextRef, JSValueToObject(ContextRef, ToValueRef(ValueHandle), nullptr));
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSValue_NativeMethods
     * Method:    nJSValueToBoolean
     * Signature: (JJ)Z
     */
    jboolean JNICALL JSValueToBoolean_Native(JNIEnv*, jclass, jlong ContextHandle, jlong ValueHandle)
    {
        return JSValueToBoolean(ToContextRef(ContextHandle), ToValueRef(ValueHandle));
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSValue_NativeMethods
     * Method:    nJSValueToNumber
     * Signature: (JJ)D
     */
    jdouble JNICALL JSValueToNumber_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jlong ValueHandle)
    {
        JSValueRef Exception = nullptr;

        const jdouble Value = JSValueToNumber(ToContextRef(ContextHandle), ToValueRef(ValueHandle), &Exception);
        if (ThrowIfJSException(Environment, ToContextRef(ContextHandle), Exception))
            return 0.0;

        return Value;
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSValue_NativeMethods
     * Method:    nJSValueToJSONString
     * Signature: (JJI)Ljava/lang/String;
     */
    jstring JNICALL JSValueToJSONString_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jlong ValueHandle,
                                               jint Indent)
    {
        JSValueRef Exception = nullptr;

        const JSStringRef JsonString = JSValueCreateJSONString(ToContextRef(ContextHandle), ToValueRef(ValueHandle),
                                                               static_cast<unsigned>(Indent), &Exception);
        if (ThrowIfJSException(Environment, ToContextRef(ContextHandle), Exception))
            return nullptr;

        if (!JsonString)
            return nullptr;

        const jstring Result = JSStringToJavaString(Environment, JsonString);
        JSStringRelease(JsonString);

        return Result;
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSValue_NativeMethods
     * Method:    nJSValueToString
     * Signature: (JJ)Ljava/lang/String;
     */
    jstring JNICALL JSValueToString_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jlong ValueHandle)
    {
        const JSStringRef StringRef =
            JSValueToStringCopy(ToContextRef(ContextHandle), ToValueRef(ValueHandle), nullptr);
        if (!StringRef)
            return Environment->NewStringUTF("");

        const jstring Result = JSStringToJavaString(Environment, StringRef);
        JSStringRelease(StringRef);

        return Result;
    }
}

static constexpr JNINativeMethod JSValueMethods[] = {
    JNI_METHOD("nJSValueGetType", "(JJ)Lme/ayydxn/luminescence/javascript/JSType;", JSValueGetType_Native),
    JNI_METHOD("nJSValueIsObject", "(JJ)Z", JSValueIsObject_Native),
    JNI_METHOD("nJSValueIsArray", "(JJ)Z", JSValueIsArray_Native),
    JNI_METHOD("nJSValueIsFunction", "(JJ)Z", JSValueIsFunction_Native),
    JNI_METHOD("nJSValueToBoolean", "(JJ)Z", JSValueToBoolean_Native),
    JNI_METHOD("nJSValueToNumber", "(JJ)D", JSValueToNumber_Native),
    JNI_METHOD("nJSValueToJSONString", "(JJI)Ljava/lang/String;", JSValueToJSONString_Native),
    JNI_METHOD("nJSValueToString", "(JJ)Ljava/lang/String;", JSValueToString_Native),
};

bool Luminescence::RegisterJSValueMethods(JNIEnv* Environment)
{
    return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/javascript/JSValue$NativeMethods", JNI_METHODS_AND_COUNT(JSValueMethods));
}
