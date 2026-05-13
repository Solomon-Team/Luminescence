#include "JSCUtilities.h"
#include "Cache/JSCCache.h"

#include <JavaScriptCore/JavaScript.h>
#include "Core/Profiling.h"

#include <vector>

namespace Luminescence::JavaScript
{
    JSStringRef JavaStringToJSString(JNIEnv* Environment, jstring String)
    {
        ZoneScoped
        
        if (!String)
            return JSStringCreateWithUTF8CString("");

        const char* Chars = Environment->GetStringUTFChars(String, nullptr);
        const JSStringRef Result = JSStringCreateWithUTF8CString(Chars);

        Environment->ReleaseStringUTFChars(String, Chars);

        return Result;
    }

    jstring JSStringToJavaString(JNIEnv* Environment, JSStringRef String)
    {
        ZoneScoped
        
        if (!String)
            return Environment->NewStringUTF("");

        const size_t MaxBytes = JSStringGetMaximumUTF8CStringSize(String);

        // Avoid a heap allocation if we can and only fall back to the heap if we really need.
        if (MaxBytes <= 256)
        {
            char Buffer[256];
            JSStringGetUTF8CString(String, Buffer, sizeof(Buffer));
            
            return Environment->NewStringUTF(Buffer);
        }

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
