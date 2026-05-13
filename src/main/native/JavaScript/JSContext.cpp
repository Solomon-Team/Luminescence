#include "JSCUtilities.h"
#include "Cache/JSCCache.h"
#include "Core/JNIUtilities.h"

#include <Ultralight/CAPI/CAPI_View.h>

using namespace Luminescence::JavaScript;

namespace
{
    /*-----------------------*/
    /* -- Utility Methods -- */
    /*-----------------------*/
    
    jobject MakeJSValue(JNIEnv* Environment, jlong ContextHandle, JSValueRef Value)
    {
        return Environment->NewObject(CJSValueCache::ClassRef, CJSValueCache::Constructor, ContextHandle, reinterpret_cast<jlong>(Value));
    }

    jobject WrapResult(JNIEnv* Environment, jlong ContextHandle, JSContextRef Context, JSValueRef Value)
    {
        if (!Value)
            return nullptr;
        
        if (JSValueIsArray(Context, Value))
            return Environment->NewObject(CJSArrayCache::ClassRef, CJSArrayCache::Constructor, ContextHandle, reinterpret_cast<jlong>(Value));
        
        if (JSValueIsObject(Context, Value))
            return Environment->NewObject(CJSObjectCache::ClassRef, CJSObjectCache::Constructor, ContextHandle, reinterpret_cast<jlong>(Value));
        
        return MakeJSValue(Environment, ContextHandle, Value);
    }
    
    /*-----------------------*/
    /*-----------------------*/
    /*-----------------------*/
    
    /*
    * Class:     me_ayydxn_luminescence_javascript_JSContext_NativeMethods
    * Method:    nJSContextMakeNumber
    * Signature: (JD)Lme/ayydxn/luminescence/javascript/JSValue;
    */
    jobject JNICALL JSContextMakeNumber_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jdouble Value)
    {
        return MakeJSValue(Environment, ContextHandle, JSValueMakeNumber(ToContextRef(ContextHandle), Value));
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSContext_NativeMethods
     * Method:    nJSContextMakeString
     * Signature: (JLjava/lang/String;)Lme/ayydxn/luminescence/javascript/JSValue;
     */
    jobject JNICALL JSContextMakeString_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jstring Value)
    {
        const JSStringRef StringRef = JavaStringToJSString(Environment, Value);
        const JSValueRef Result = JSValueMakeString(ToContextRef(ContextHandle), StringRef);
        
        JSStringRelease(StringRef);
        
        return MakeJSValue(Environment, ContextHandle, Result);
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSContext_NativeMethods
     * Method:    nJSContextMakeBoolean
     * Signature: (JZ)Lme/ayydxn/luminescence/javascript/JSValue;
     */
    jobject JNICALL JSContextMakeBoolean_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jboolean Value)
    {
        return MakeJSValue(Environment, ContextHandle, JSValueMakeBoolean(ToContextRef(ContextHandle), static_cast<bool>(Value)));
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSContext_NativeMethods
     * Method:    nJSContextMakeUndefined
     * Signature: (J)Lme/ayydxn/luminescence/javascript/JSValue;
     */
    jobject JNICALL JSContextMakeUndefined_Native(JNIEnv* Environment, jclass, jlong ContextHandle)
    {
        return MakeJSValue(Environment, ContextHandle, JSValueMakeUndefined(ToContextRef(ContextHandle)));
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSContext_NativeMethods
     * Method:    nJSContextMakeNull
     * Signature: (J)Lme/ayydxn/luminescence/javascript/JSValue;
     */
    jobject JNICALL JSContextMakeNull_Native(JNIEnv* Environment, jclass, jlong ContextHandle)
    {
        return MakeJSValue(Environment, ContextHandle, JSValueMakeNull(ToContextRef(ContextHandle)));
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSContext_NativeMethods
     * Method:    nJSContextGetGlobalObject
     * Signature: (J)Lme/ayydxn/luminescence/javascript/JSObject;
     */
    jobject JNICALL JSContextGetGlobalObject_Native(JNIEnv* Environment, jclass, jlong ContextHandle)
    {
        JSObjectRef GlobalObject = JSContextGetGlobalObject(ToContextRef(ContextHandle));
        
        return Environment->NewObject(CJSObjectCache::ClassRef, CJSObjectCache::Constructor,ContextHandle, reinterpret_cast<jlong>(GlobalObject));
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSContext_NativeMethods
     * Method:    nJSContextEvaluate
     * Signature: (JLjava/lang/String;Ljava/lang/String;I)Lme/ayydxn/luminescence/javascript/JSValue;
     */
    jobject JNICALL JSContextEvaluate_Native(JNIEnv* Environment, jclass, jlong ContextHandle, jstring Script, jstring SourceURL, jint Line)
    {
        const auto ContextRef = ToContextRef(ContextHandle);
        const JSStringRef JavaScriptScript = JavaStringToJSString(Environment, Script);
        const JSStringRef JavaScriptSource = SourceURL ? JavaStringToJSString(Environment, SourceURL) : nullptr;
        JSValueRef Exception = nullptr;
        const JSValueRef Result = JSEvaluateScript(ContextRef, JavaScriptScript, nullptr, JavaScriptSource, Line, &Exception);
        
        JSStringRelease(JavaScriptScript);
        
        if (JavaScriptSource)
            JSStringRelease(JavaScriptSource);
        
        if (ThrowIfJSException(Environment, ContextRef, Exception))
            return nullptr;
        
        return WrapResult(Environment, ContextHandle, ContextRef, Result);
    }

    /*
     * Class:     me_ayydxn_luminescence_javascript_JSContext_NativeMethods
     * Method:    nJSContextUnlock
     * Signature: (J)V
     */
    void JNICALL JSContextUnlock_Native(JNIEnv*, jclass, jlong ViewHandle)
    {
        if (ViewHandle == 0)
            ulViewUnlockJSContext(reinterpret_cast<ULView>(ViewHandle));
    }
}

static constexpr JNINativeMethod JSContextMethods[] = {
    JNI_METHOD("nJSContextMakeNumber", "(JD)Lme/ayydxn/luminescence/javascript/JSValue;", JSContextMakeNumber_Native),
    JNI_METHOD("nJSContextMakeString", "(JLjava/lang/String;)Lme/ayydxn/luminescence/javascript/JSValue;", JSContextMakeString_Native),
    JNI_METHOD("nJSContextMakeBoolean", "(JZ)Lme/ayydxn/luminescence/javascript/JSValue;", JSContextMakeBoolean_Native),
    JNI_METHOD("nJSContextMakeUndefined", "(J)Lme/ayydxn/luminescence/javascript/JSValue;", JSContextMakeUndefined_Native),
    JNI_METHOD("nJSContextMakeNull", "(J)Lme/ayydxn/luminescence/javascript/JSValue;", JSContextMakeNull_Native),
    JNI_METHOD("nJSContextGetGlobalObject", "(J)Lme/ayydxn/luminescence/javascript/JSObject;", JSContextGetGlobalObject_Native),
    JNI_METHOD("nJSContextEvaluate", "(JLjava/lang/String;Ljava/lang/String;I)Lme/ayydxn/luminescence/javascript/JSValue;", JSContextEvaluate_Native),
    JNI_METHOD("nJSContextUnlock", "(J)V", JSContextUnlock_Native),
};

bool Luminescence::RegisterJSContextMethods(JNIEnv* Environment)
{
    return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/javascript/JSContext$NativeMethods", JNI_METHODS_AND_COUNT(JSContextMethods));
}
