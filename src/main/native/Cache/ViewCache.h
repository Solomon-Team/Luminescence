#pragma once

#include "Core/ScopedLocalRef.h"

#include <jni.h>

namespace Luminescence
{
	struct CViewCache
	{
	    inline static jclass ClassRef = nullptr;
	    inline static jclass CursorClassRef = nullptr;
	    inline static jclass MessageSourceClassRef = nullptr;
	    inline static jclass MessageLevelClassRef = nullptr;

	    // Fields
	    inline static jfieldID HandleFieldID = nullptr;

	    // Static Methods
	    inline static jmethodID CursorFromNativeMethodID = nullptr;
	    inline static jmethodID MessageSourceFromNativeMethodID = nullptr;
	    inline static jmethodID MessageLevelFromNativeMethodID = nullptr;

	    // ULViewListener Callback Methods
		inline static jmethodID OnTitleChangeMethodID = nullptr;
	    inline static jmethodID OnURLChangeMethodID = nullptr;
	    inline static jmethodID OnTooltipChangeMethodID = nullptr;
	    inline static jmethodID OnCursorChangeMethodID = nullptr;
		inline static jmethodID OnConsoleMessageAddedMethodID = nullptr;
	    inline static jmethodID OnChildViewCreatedMethodID = nullptr;
	    inline static jmethodID OnInspectorViewCreatedMethodID = nullptr;
	    inline static jmethodID OnLoadingBeginMethodID = nullptr;
	    inline static jmethodID OnLoadingFinishMethodID = nullptr;
	    inline static jmethodID OnLoadingFailMethodID = nullptr;
	    inline static jmethodID OnWindowObjectReadyMethodID = nullptr;
	    inline static jmethodID OnDOMReadyMethodID = nullptr;
	    inline static jmethodID OnUpdateHistoryMethodID = nullptr;
	
	    static bool InitializeCache(JNIEnv* Environment)
	    {
	        if (bIsInitialized)
	            return true;

	        const CScopedLocalRef<jclass> LocalClass(Environment, Environment->FindClass("me/ayydxn/luminescence/view/ULView"));
	        if (!LocalClass)
	            return false;
	
	        ClassRef = (jclass) Environment->NewGlobalRef(LocalClass.Get());
	        HandleFieldID = Environment->GetFieldID(ClassRef, "handle", "J");

	        const CScopedLocalRef<jclass> LocalCursorClass(Environment, Environment->FindClass("me/ayydxn/luminescence/view/ULCursor"));
	        CursorClassRef = (jclass) Environment->NewGlobalRef(LocalCursorClass.Get());
	        CursorFromNativeMethodID = Environment->GetStaticMethodID(CursorClassRef, "fromNativeValue", "(I)Lme/ayydxn/luminescence/view/ULCursor;");

	        const CScopedLocalRef<jclass> LocalMessageSourceClass(Environment, Environment->FindClass("me/ayydxn/luminescence/console/ULMessageSource"));
	        MessageSourceClassRef = (jclass) Environment->NewGlobalRef(LocalMessageSourceClass.Get());
	        MessageSourceFromNativeMethodID = Environment->GetStaticMethodID(MessageSourceClassRef, "fromNativeValue", "(I)Lme/ayydxn/luminescence/console/ULMessageSource;");

	        const CScopedLocalRef<jclass> LocalMessageLevelClass(Environment, Environment->FindClass("me/ayydxn/luminescence/console/ULMessageLevel"));
	        MessageLevelClassRef = (jclass) Environment->NewGlobalRef(LocalMessageLevelClass.Get());
	        MessageLevelFromNativeMethodID = Environment->GetStaticMethodID(MessageLevelClassRef, "fromNativeValue", "(I)Lme/ayydxn/luminescence/console/ULMessageLevel;");
	    	
	        const CScopedLocalRef<jclass> ListenerClass(Environment, Environment->FindClass("me/ayydxn/luminescence/view/ULViewListener")); 
	        if (ListenerClass)
	        {
	        	OnTitleChangeMethodID = Environment->GetMethodID(ListenerClass.Get(), "onTitleChange", "(Ljava/lang/String;)V");
	        	OnURLChangeMethodID = Environment->GetMethodID(ListenerClass.Get(), "onURLChange", "(Ljava/lang/String;)V");
	        	OnTooltipChangeMethodID = Environment->GetMethodID(ListenerClass.Get(), "onTooltipChange", "(Ljava/lang/String;)V");
	        	OnCursorChangeMethodID = Environment->GetMethodID(ListenerClass.Get(), "onCursorChange", "(Lme/ayydxn/luminescence/view/ULCursor;)V");
	        	
	        	OnConsoleMessageAddedMethodID = Environment->GetMethodID(ListenerClass.Get(), "onConsoleMessageAdded",
	        		"(Lme/ayydxn/luminescence/console/ULMessageSource;Lme/ayydxn/luminescence/console/ULMessageLevel;Ljava/lang/String;IILjava/lang/String;)V");
	        	
	            OnChildViewCreatedMethodID = Environment->GetMethodID(ListenerClass.Get(), "onChildViewCreated", 
	                "(Ljava/lang/String;Ljava/lang/String;ZLme/ayydxn/luminescence/geometry/ULIntRect;)Lme/ayydxn/luminescence/view/ULView;");
	            
	            OnInspectorViewCreatedMethodID = Environment->GetMethodID(ListenerClass.Get(), "onInspectorViewCreated", 
	                "(ZLjava/lang/String;)Lme/ayydxn/luminescence/view/ULView;");
	        	
	        	OnLoadingBeginMethodID = Environment->GetMethodID(ListenerClass.Get(), "onLoadingBegin", "(JZLjava/lang/String;)V");
	        	OnLoadingFinishMethodID = Environment->GetMethodID(ListenerClass.Get(), "onLoadingFinish", "(JZLjava/lang/String;)V");
	        	
	        	OnLoadingFailMethodID = Environment->GetMethodID(ListenerClass.Get(), "onLoadingFail",
	        		"(JZLjava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
	        	
	        	OnWindowObjectReadyMethodID = Environment->GetMethodID(ListenerClass.Get(), "onWindowObjectReady", "(JZLjava/lang/String;)V");
	        	OnDOMReadyMethodID = Environment->GetMethodID(ListenerClass.Get(), "onDOMReady", "(JZLjava/lang/String;)V");
	        	OnUpdateHistoryMethodID = Environment->GetMethodID(ListenerClass.Get(), "onUpdateHistory", "()V");
	        }
	
	        bIsInitialized = (ClassRef != nullptr && CursorClassRef != nullptr && MessageSourceClassRef != nullptr && MessageLevelClassRef != nullptr);
	    	
	        return bIsInitialized;
	    }
	
	    static void ClearCache(JNIEnv* Environment)
	    {
	        if (ClassRef) Environment->DeleteGlobalRef(ClassRef);
	        if (CursorClassRef) Environment->DeleteGlobalRef(CursorClassRef);
	        if (MessageSourceClassRef) Environment->DeleteGlobalRef(MessageSourceClassRef);
	        if (MessageLevelClassRef) Environment->DeleteGlobalRef(MessageLevelClassRef);
	            
	        ClassRef = nullptr;
	        CursorClassRef = nullptr;
	        MessageSourceClassRef = nullptr;
	        MessageLevelClassRef = nullptr;
	    	
	        bIsInitialized = false;
	    }
	
	    static ULView GetNativeHandle(JNIEnv* Environment, jobject JavaView)
	    {
	        if (!JavaView)
	        	return nullptr;
	    	
	        return reinterpret_cast<ULView>(Environment->GetLongField(JavaView, HandleFieldID));
	    }
	private:
	    inline static bool bIsInitialized = false;
	};
}