#include "JSCUtilities.h"
#include "Cache/JSCCache.h"
#include "Core/JNIUtilities.h"

using namespace Luminescence::JavaScript;

namespace
{
    /*-----------------------*/
    /* -- Utility Methods -- */
    /*-----------------------*/

    // Wraps a JSValueRef into the most specific Java type available
    jobject WrapValue(JNIEnv* Environment, jlong ContextHandle, JSContextRef Context, JSValueRef Value)
    {
        if (JSValueIsArray(Context, Value))
            return Environment->NewObject(CJSArrayCache::ClassRef, CJSArrayCache::Constructor, ContextHandle, reinterpret_cast<jlong>(Value));

        if (JSValueIsObject(Context, Value))
            return Environment->NewObject(CJSObjectCache::ClassRef, CJSObjectCache::Constructor, ContextHandle, reinterpret_cast<jlong>(Value));

        return Environment->NewObject(CJSValueCache::ClassRef, CJSValueCache::Constructor, ContextHandle, reinterpret_cast<jlong>(Value));
    }

    /*-----------------------*/
    /*-----------------------*/
    /*-----------------------*/

    /*
    * Class:     me_ayydxn_luminescence_javascript_JSObject_NativeMethods
    * Method:    nJSObjectGetProperty
    * Signature: (JJLjava/lang/String;)Lme/ayydxn/luminescence/javascript/JSValue;
    */
    jobject JNICALL JSObjectGetProperty_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jlong ObjectHandle, jstring Name)
    {
        const auto Ctx = ToContextRef(ContextHandle);
        const JSStringRef PropertyName = JavaStringToJSString(Environment, Name);
        JSValueRef Exception = nullptr;
        const JSValueRef Result = JSObjectGetProperty(Ctx, ToObjectRef(ObjectHandle), PropertyName, &Exception);

        JSStringRelease(PropertyName);

        if (ThrowIfJSException(Environment, Ctx, Exception))
            return nullptr;

        return WrapValue(Environment, ContextHandle, Ctx, Result);
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSObject_NativeMethods
     * Method:    nJSObjectSetProperty
     * Signature: (JJLjava/lang/String;J)V
     */
    void JNICALL JSObjectSetProperty_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jlong ObjectHandle, jstring Name, jlong ValueHandle)
    {
        const auto ContextRef = ToContextRef(ContextHandle);
        const JSStringRef PropertyName = JavaStringToJSString(Environment, Name);
        JSValueRef Exception = nullptr;

        JSObjectSetProperty(ContextRef, ToObjectRef(ObjectHandle), PropertyName, ToValueRef(ValueHandle),
                            kJSPropertyAttributeNone, &Exception);

        JSStringRelease(PropertyName);

        ThrowIfJSException(Environment, ContextRef, Exception);
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSObject_NativeMethods
     * Method:    nJSObjectHasProperty
     * Signature: (JJLjava/lang/String;)Z
     */
    jboolean JNICALL JSObjectHasProperty_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jlong ObjectHandle, jstring Name)
    {
        const JSStringRef PropertyName = JavaStringToJSString(Environment, Name);
        const bool Result = JSObjectHasProperty(ToContextRef(ContextHandle), ToObjectRef(ObjectHandle), PropertyName);

        JSStringRelease(PropertyName);

        return Result;
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSObject_NativeMethods
     * Method:    nJSObjectDeleteProperty
     * Signature: (JJLjava/lang/String;)Z
     */
    jboolean JNICALL JSObjectDeleteProperty_Native(JNIEnv* Env, jclass, jlong CtxH, jlong ObjH, jstring Name)
    {
        const auto ContextRef = ToContextRef(CtxH);
        JSStringRef PropertyName = JavaStringToJSString(Env, Name);
        JSValueRef Exception = nullptr;
        const bool Result = JSObjectDeleteProperty(ContextRef, ToObjectRef(ObjH), PropertyName, &Exception);

        JSStringRelease(PropertyName);

        if (ThrowIfJSException(Env, ContextRef, Exception))
            return JNI_FALSE;

        return Result;
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSObject_NativeMethods
     * Method:    nJSObjectGetPropertyAtIndex
     * Signature: (JJI)Lme/ayydxn/luminescence/javascript/JSValue;
     */
    jobject JNICALL JSObjectGetPropertyAtIndex_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jlong ObjectHandle, jint Index)
    {
        const auto ContextRef = ToContextRef(ContextHandle);
        JSValueRef Exception = nullptr;
        const JSValueRef Result = JSObjectGetPropertyAtIndex(ContextRef, ToObjectRef(ObjectHandle),
                                                             static_cast<unsigned>(Index), &Exception);

        if (ThrowIfJSException(Environment, ContextRef, Exception))
            return nullptr;

        return WrapValue(Environment, ContextHandle, ContextRef, Result);
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSObject_NativeMethods
     * Method:    nJSObjectSetPropertyAtIndex
     * Signature: (JJIJ)V
     */
    void JNICALL JSObjectSetPropertyAtIndex_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jlong ObjectHandle, jint Index, jlong ValueHandle)
    {
        const auto ContextRef = ToContextRef(ContextHandle);
        JSValueRef Exception = nullptr;

        JSObjectSetPropertyAtIndex(ContextRef, ToObjectRef(ObjectHandle), static_cast<unsigned>(Index),
                                   ToValueRef(ValueHandle), &Exception);

        ThrowIfJSException(Environment, ContextRef, Exception);
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSObject_NativeMethods
     * Method:    nJSObjectGetPropertyNames
     * Signature: (JJ)[Ljava/lang/String;
     */
    jobjectArray JNICALL JSObjectGetPropertyNames_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jlong ObjectHandle)
    {
        const JSPropertyNameArrayRef Names = JSObjectCopyPropertyNames(ToContextRef(ContextHandle), ToObjectRef(ObjectHandle));
        const size_t Count = JSPropertyNameArrayGetCount(Names);
        const jobjectArray Result = Environment->NewObjectArray(static_cast<jsize>(Count), CStringCache::ClassRef, nullptr);
        
        for (size_t Index = 0; Index < Count; ++Index)
        {
            const jstring String = JSStringToJavaString(Environment, JSPropertyNameArrayGetNameAtIndex(Names, Index));
            
            Environment->SetObjectArrayElement(Result, static_cast<jsize>(Index), String);
            Environment->DeleteLocalRef(String);
        }
        
        JSPropertyNameArrayRelease(Names);
        
        return Result;
    }
}

static constexpr JNINativeMethod JSObjectMethods[] = {
    JNI_METHOD("nJSObjectGetProperty", "(JJLjava/lang/String;)Lme/ayydxn/luminescence/javascript/JSValue;", JSObjectGetProperty_Native),
    JNI_METHOD("nJSObjectSetProperty", "(JJLjava/lang/String;J)V", JSObjectSetProperty_Native),
    JNI_METHOD("nJSObjectHasProperty", "(JJLjava/lang/String;)Z", JSObjectHasProperty_Native),
    JNI_METHOD("nJSObjectDeleteProperty", "(JJLjava/lang/String;)Z", JSObjectDeleteProperty_Native),
    JNI_METHOD("nJSObjectGetPropertyAtIndex", "(JJI)Lme/ayydxn/luminescence/javascript/JSValue;", JSObjectGetPropertyAtIndex_Native),
    JNI_METHOD("nJSObjectSetPropertyAtIndex", "(JJIJ)V", JSObjectSetPropertyAtIndex_Native),
    JNI_METHOD("nJSObjectGetPropertyNames", "(JJ)[Ljava/lang/String;", JSObjectGetPropertyNames_Native),
};

bool Luminescence::RegisterJSObjectMethods(JNIEnv* Environment)
{
    return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/javascript/JSObject$NativeMethods", JNI_METHODS_AND_COUNT(JSObjectMethods));
}
