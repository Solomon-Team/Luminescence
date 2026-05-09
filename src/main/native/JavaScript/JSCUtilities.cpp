#include "JSCUtilities.h"
#include "Cache/JSCCache.h"

#include <JavaScriptCore/JavaScript.h>

#include <vector>

namespace Luminescence::JavaScript
{
    JSStringRef JavaStringToJSString(JNIEnv* Environment, jstring String)
    {
        if (!String)
            return JSStringCreateWithUTF8CString("");

        const char* Chars = Environment->GetStringUTFChars(String, nullptr);
        const JSStringRef Result = JSStringCreateWithUTF8CString(Chars);

        Environment->ReleaseStringUTFChars(String, Chars);

        return Result;
    }

    jstring JSStringToJavaString(JNIEnv* Environment, JSStringRef String)
    {
        if (!String)
            return Environment->NewStringUTF("");
        
        const size_t MaxBytes = JSStringGetMaximumUTF8CStringSize(String);
        std::vector<char> Buffer(MaxBytes);
        
        JSStringGetUTF8CString(String, Buffer.data(), MaxBytes);
        
        return Environment->NewStringUTF(Buffer.data());
    }

    bool ThrowIfJSException(JNIEnv* Environment, JSContextRef Context, JSValueRef Exception)
    {
        if (!Exception)
            return false;
        
        const jlong ContextHandle   = reinterpret_cast<jlong>(Context);
        const jlong ValueHandle = reinterpret_cast<jlong>(Exception);
        const jobject JavaValue = Environment->NewObject(CJSValueCache::ClassRef, CJSValueCache::Constructor, ContextHandle, ValueHandle);
        const jobject JavaException = Environment->NewObject(CJSExceptionCache::ClassRef, CJSExceptionCache::ConstructorWithJSValue, JavaValue);
        
        Environment->Throw((jthrowable) JavaException);
        
        return true;
    }
}
