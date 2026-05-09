#pragma once

#include "Core/CallbackAdapter.h"

#include <JavaScriptCore/JavaScript.h>

namespace Luminescence
{
    class CJSFunctionCallbackAdapter : public ICallbackAdapter
    {
    public:
        CJSFunctionCallbackAdapter(JNIEnv* Environment, jobject CallbackInstance);
        ~CJSFunctionCallbackAdapter() override = default;
        
        JSValueRef Invoke(JSContextRef ContextRef, JSObjectRef Function, JSObjectRef ThisObject, size_t ArgumentCount, const JSValueRef* Arguments, JSValueRef* Exception) const;

        static JSClassRef GetJavaFunctionClass();
    };
}
