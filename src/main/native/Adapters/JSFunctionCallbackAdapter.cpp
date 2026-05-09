#include "JSFunctionCallbackAdapter.h"
#include "Cache/JSCCache.h"
#include "JavaScript/JSCUtilities.h"

using namespace Luminescence;
using namespace Luminescence::JavaScript;

namespace 
{
    JSValueRef JSCallback_Trampoline(JSContextRef ContextRef, JSObjectRef Function, JSObjectRef ThisObject, size_t ArgumentCount, const JSValueRef Arguments[],
                                     JSValueRef* Exception)
    {
        void* PrivateData = JSObjectGetPrivate(Function);
        if (!PrivateData)
            return JSValueMakeUndefined(ContextRef);

        const auto* CallbackAdapter = static_cast<CJSFunctionCallbackAdapter*>(PrivateData);
        return CallbackAdapter->Invoke(ContextRef, Function, ThisObject, ArgumentCount, Arguments, Exception);
    }

    void JSFunction_Finalize(JSObjectRef Object)
    {
        if (void* PrivateData = JSObjectGetPrivate(Object))
            delete static_cast<CJSFunctionCallbackAdapter*>(PrivateData);
    }
}

namespace Luminescence
{
    CJSFunctionCallbackAdapter::CJSFunctionCallbackAdapter(JNIEnv* Environment, jobject CallbackInstance)
        : ICallbackAdapter(Environment, CallbackInstance) {}

    JSValueRef CJSFunctionCallbackAdapter::Invoke(JSContextRef ContextRef, JSObjectRef Function, JSObjectRef ThisObject, size_t ArgumentCount, const JSValueRef* Arguments,
                                                  JSValueRef* Exception) const
    {
        JNIEnv* Environment = GetJNIEnvironment();
        if (!Environment)
            return JSValueMakeUndefined(ContextRef);
    
        const jlong CtxH = reinterpret_cast<jlong>(ContextRef);
    
        // Wrap context — viewHandle=0 intentionally: callers must not call close() on this
        const jobject JavaContext = Environment->NewObject(CJSContextCache::ClassRef, CJSContextCache::Constructor, static_cast<jlong>(0), CtxH);
        const jobject JavaThis = Environment->NewObject(CJSObjectCache::ClassRef, CJSObjectCache::Constructor, CtxH, reinterpret_cast<jlong>(ThisObject));

        const jobjectArray JavaArguments = Environment->NewObjectArray(static_cast<jsize>(ArgumentCount), CJSValueCache::ClassRef, nullptr);
        for (size_t Index = 0; Index < ArgumentCount; ++Index)
        {
            const jobject Value = Environment->NewObject(CJSValueCache::ClassRef, CJSValueCache::Constructor, CtxH, reinterpret_cast<jlong>(Arguments[Index]));
            
            Environment->SetObjectArrayElement(JavaArguments, static_cast<jsize>(Index), Value);
            Environment->DeleteLocalRef(Value);
        }

        const jobject Result = Environment->CallObjectMethod(m_JavaImplementation, CJSFunctionCache::CallbackInvokeID, JavaContext, JavaThis, JavaArguments);
    
        Environment->DeleteLocalRef(JavaContext);
        Environment->DeleteLocalRef(JavaThis);
        Environment->DeleteLocalRef(JavaArguments);
    
        if (Environment->ExceptionCheck())
        {
            const jthrowable Thrown = Environment->ExceptionOccurred();
            Environment->ExceptionClear();

            const jobject JavaScriptValue = Environment->CallObjectMethod(Thrown, CJSExceptionCache::GetJSValueMethodID);
            Environment->DeleteLocalRef(Thrown);
            
            if (JavaScriptValue)
            {
                const jlong Handle = Environment->GetLongField(JavaScriptValue, CJSValueCache::ValueHandleFieldID);
                Environment->DeleteLocalRef(JavaScriptValue);
                
                *Exception = ToValueRef(Handle);
            }
            else
            {
                const JSStringRef Message = JSStringCreateWithUTF8CString("Java Exception in JSCallback");
                *Exception = JSValueMakeString(ContextRef, Message);
                
                JSStringRelease(Message);
            }
            
            return JSValueMakeUndefined(ContextRef);
        }
    
        if (!Result)
            return JSValueMakeUndefined(ContextRef);
        
        const jlong ResultHandle = Environment->GetLongField(Result, CJSValueCache::ValueHandleFieldID);
        
        Environment->DeleteLocalRef(Result);
        
        return ToValueRef(ResultHandle);
    }

    JSClassRef CJSFunctionCallbackAdapter::GetJavaFunctionClass()
    {
        static JSClassRef SharedClass = []() {
            JSClassDefinition ClassDefinition = kJSClassDefinitionEmpty;
            ClassDefinition.callAsFunction = JSCallback_Trampoline;
            ClassDefinition.finalize = JSFunction_Finalize;
            
            return JSClassCreate(&ClassDefinition);
        }();
        
        return SharedClass;
    }
}
