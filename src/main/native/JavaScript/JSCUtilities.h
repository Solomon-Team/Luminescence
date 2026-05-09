#pragma once

#include <jni.h>
#include <JavaScriptCore/JavaScript.h>

namespace Luminescence::JavaScript
{
    inline JSContextRef ToContextRef(jlong Handle) { return reinterpret_cast<JSContextRef>(Handle); }
    inline JSValueRef ToValueRef(jlong Handle) { return reinterpret_cast<JSValueRef>(Handle); }
    inline JSObjectRef ToObjectRef(jlong Handle) { return reinterpret_cast<JSObjectRef>(Handle); }
    inline jlong ToHandle(JSValueRef ValueRef) { return reinterpret_cast<jlong>(ValueRef); }
    inline jlong ToHandle(JSObjectRef ObjectRef) { return reinterpret_cast<jlong>(ObjectRef); }

    JSStringRef JavaStringToJSString(JNIEnv* Environment, jstring String);
    jstring JSStringToJavaString(JNIEnv* Environment, JSStringRef String);
    bool ThrowIfJSException(JNIEnv* Environment, JSContextRef Context, JSValueRef Exception);
}
