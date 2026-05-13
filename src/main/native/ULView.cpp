#include "Adapters/ViewCallbackAdapter.h"
#include "Cache/BitmapFormatCache.h"
#include "Cache/RectCache.h"
#include "Cache/RenderTargetCache.h"
#include "Cache/ViewCache.h"
#include "Core/CallbackAdapterRegistry.h"
#include "Core/JNIUtilities.h"
#include "Core/Profiling.h"

#include <Ultralight/CAPI/CAPI_View.h>

namespace
{
	using namespace Luminescence;

	void OnChangeTitle_Trampoline(void*, ULView Caller, ULString Title)
	{
		ZoneScoped
		
		if (const auto* ViewCallbackAdapter = static_cast<CViewCallbackAdapter*>(CCallbackAdapterRegistry::GetInstance().Find(reinterpret_cast<uintptr_t>(Caller))))
			ViewCallbackAdapter->OnChangeTitle(Title);
	}

	void OnChangeURL_Trampoline(void*, ULView Caller, ULString URL)
	{
		ZoneScoped
		
		if (const auto* ViewCallbackAdapter = static_cast<CViewCallbackAdapter*>(CCallbackAdapterRegistry::GetInstance().Find(reinterpret_cast<uintptr_t>(Caller))))
			ViewCallbackAdapter->OnChangeURL(URL);
	}

	void OnTooltipChange_Trampoline(void*, ULView Caller, ULString Tooltip)
	{
		ZoneScoped
		
		if (const auto* ViewCallbackAdapter = static_cast<CViewCallbackAdapter*>(CCallbackAdapterRegistry::GetInstance().Find(reinterpret_cast<uintptr_t>(Caller))))
			ViewCallbackAdapter->OnTooltipChange(Tooltip);
	}

	void OnCursorChange_Trampoline(void*, ULView Caller, ULCursor Cursor)
	{
		ZoneScoped
		
		if (const auto* ViewCallbackAdapter = static_cast<CViewCallbackAdapter*>(CCallbackAdapterRegistry::GetInstance().Find(reinterpret_cast<uintptr_t>(Caller))))
			ViewCallbackAdapter->OnCursorChange(Cursor);
	}

	void OnConsoleMessageAdded_Trampoline(void*, ULView Caller, ULMessageSource Source, ULMessageLevel Level, ULString Message, unsigned int LineNumber, unsigned int ColumnNumber, ULString SourceID)
	{
		ZoneScoped
		
		if (const auto* ViewCallbackAdapter = static_cast<CViewCallbackAdapter*>(CCallbackAdapterRegistry::GetInstance().Find(reinterpret_cast<uintptr_t>(Caller))))
			ViewCallbackAdapter->OnConsoleMessageAdded(Source, Level, Message, LineNumber, ColumnNumber, SourceID);
	}

	ULView OnChildViewCreated_Trampoline(void*, ULView Caller, ULString OpenerURL, ULString TargetURL, bool bIsPopup, ULIntRect PopupRect)
	{
		ZoneScoped
		
		if (const auto* ViewCallbackAdapter = static_cast<CViewCallbackAdapter*>(CCallbackAdapterRegistry::GetInstance().Find(reinterpret_cast<uintptr_t>(Caller))))
			return ViewCallbackAdapter->OnChildViewCreated(OpenerURL, TargetURL, bIsPopup, PopupRect);
		
		return nullptr;
	}
	
	ULView OnInspectorViewCreated_Trampoline(void*, ULView Caller, bool bIsLocal, ULString InspectedURL)
	{
		ZoneScoped
		
		if (const auto* ViewCallbackAdapter = static_cast<CViewCallbackAdapter*>(CCallbackAdapterRegistry::GetInstance().Find(reinterpret_cast<uintptr_t>(Caller))))
			return ViewCallbackAdapter->OnInspectorViewCreated(bIsLocal, InspectedURL);
		
		return nullptr;
	}
	
	void OnLoadingBegin_Trampoline(void*, ULView Caller, unsigned long long FrameID, bool bIsMainFrame, ULString URL)
	{
		ZoneScoped
		
		if (const auto* ViewCallbackAdapter = static_cast<CViewCallbackAdapter*>(CCallbackAdapterRegistry::GetInstance().Find(reinterpret_cast<uintptr_t>(Caller))))
			ViewCallbackAdapter->OnLoadingBegin(FrameID, bIsMainFrame, URL);
	}
	
	void OnLoadingFinish_Trampoline(void*, ULView Caller, unsigned long long FrameID, bool bIsMainFrame, ULString URL)
	{
		ZoneScoped
		
		if (const auto* ViewCallbackAdapter = static_cast<CViewCallbackAdapter*>(CCallbackAdapterRegistry::GetInstance().Find(reinterpret_cast<uintptr_t>(Caller))))
			ViewCallbackAdapter->OnLoadingFinish(FrameID, bIsMainFrame, URL);
	}
	
	void OnLoadingFail_Trampoline(void*, ULView Caller, unsigned long long FrameID, bool bIsMainFrame, ULString URL, ULString Description, ULString ErrorDomain, int ErrorCode)
	{
		ZoneScoped
		
		if (const auto* ViewCallbackAdapter = static_cast<CViewCallbackAdapter*>(CCallbackAdapterRegistry::GetInstance().Find(reinterpret_cast<uintptr_t>(Caller))))
			ViewCallbackAdapter->OnLoadingFail(FrameID, bIsMainFrame, URL, Description, ErrorDomain, ErrorCode);
	}
	
	void OnWindowObjectReady_Trampoline(void*, ULView Caller, unsigned long long FrameID, bool bIsMainFrame, ULString URL)
	{
		ZoneScoped

		if (const auto* ViewCallbackAdapter = static_cast<CViewCallbackAdapter*>(CCallbackAdapterRegistry::GetInstance().Find(reinterpret_cast<uintptr_t>(Caller))))
			ViewCallbackAdapter->OnWindowObjectReady(FrameID, bIsMainFrame, URL);
	}
	
	void OnDOMReady_Trampoline(void*, ULView Caller, unsigned long long FrameID, bool bIsMainFrame, ULString URL)
	{
		ZoneScoped
		
		if (const auto* ViewCallbackAdapter = static_cast<CViewCallbackAdapter*>(CCallbackAdapterRegistry::GetInstance().Find(reinterpret_cast<uintptr_t>(Caller))))
			ViewCallbackAdapter->OnDOMReady(FrameID, bIsMainFrame, URL);
	}
	
	void OnUpdateHistory_Trampoline(void*, ULView Caller)
	{
		ZoneScoped
		
		if (const auto* ViewCallbackAdapter = static_cast<CViewCallbackAdapter*>(CCallbackAdapterRegistry::GetInstance().Find(reinterpret_cast<uintptr_t>(Caller))))
			ViewCallbackAdapter->OnUpdateHistory();
	}
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulCreateView
 * Signature: (JIIJJ)J
 */
jlong JNICALL ULCreateView_Native(JNIEnv*, jclass, jlong RendererHandle, jint Width, jint Height, jlong ViewConfigHandle, jlong SessionHandle)
{
	ZoneScoped
	
	const auto Renderer = reinterpret_cast<ULRenderer>(RendererHandle);
	const auto ViewConfig = reinterpret_cast<ULViewConfig>(ViewConfigHandle);
	const auto Session = reinterpret_cast<ULSession>(SessionHandle);

	const auto View = ulCreateView(Renderer, static_cast<unsigned int>(Width), static_cast<unsigned int>(Height), ViewConfig, Session);

	return reinterpret_cast<jlong>(View);
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulDestroyView
 * Signature: (J)V
 */
void JNICALL ULDestroyView_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	ulDestroyView(reinterpret_cast<ULView>(ViewHandle));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewGetURL
 * Signature: (J)Ljava/lang/String;
 */
jstring JNICALL ULViewGetURL_Native(JNIEnv* Environment, jclass, jlong ViewHandle)
{
	ZoneScoped
	
	const auto View = reinterpret_cast<ULView>(ViewHandle);
	const auto ViewURL = ulViewGetURL(View);
	const auto ViewURLData = ulStringGetData(ViewURL);

	return Environment->NewStringUTF(ViewURLData);
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewGetTitle
 * Signature: (J)Ljava/lang/String;
 */
jstring JNICALL ULViewGetTitle_Native(JNIEnv* Environment, jclass, jlong ViewHandle)
{
	ZoneScoped

	const auto View = reinterpret_cast<ULView>(ViewHandle);
	const auto ViewTitle = ulViewGetTitle(View);
	const auto ViewTitleData = ulStringGetData(ViewTitle);

	return Environment->NewStringUTF(ViewTitleData);
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewGetWidth
 * Signature: (J)I
 */
jint JNICALL ULViewGetWidth_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	return static_cast<int>(ulViewGetWidth(reinterpret_cast<ULView>(ViewHandle)));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewGetHeight
 * Signature: (J)I
 */
jint JNICALL ULViewGetHeight_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	return static_cast<int>(ulViewGetHeight(reinterpret_cast<ULView>(ViewHandle)));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewGetDisplayId
 * Signature: (J)I
 */
jint JNICALL ULViewGetDisplayID_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	return static_cast<int>(ulViewGetDisplayId(reinterpret_cast<ULView>(ViewHandle)));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewSetDisplayId
 * Signature: (JI)V
 */
void JNICALL ULViewSetDisplayID_Native(JNIEnv*, jclass, jlong ViewHandle, jint DisplayID)
{
	ZoneScoped

	const auto View = reinterpret_cast<ULView>(ViewHandle);

	ulViewSetDisplayId(View, static_cast<unsigned int>(DisplayID));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewGetDeviceScale
 * Signature: (J)D
 */
jdouble JNICALL ULViewGetDeviceScale_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	return ulViewGetDeviceScale(reinterpret_cast<ULView>(ViewHandle));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewSetDeviceScale
 * Signature: (JD)V
 */
void JNICALL ULViewSetDeviceScale_Native(JNIEnv*, jclass, jlong ViewHandle, jdouble DeviceScale)
{
	ZoneScoped

	const auto View = reinterpret_cast<ULView>(ViewHandle);

	ulViewSetDeviceScale(View, DeviceScale);
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewIsAccelerated
 * Signature: (J)Z
 */
jboolean JNICALL ULViewIsAccelerated_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	return ulViewIsAccelerated(reinterpret_cast<ULView>(ViewHandle));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewIsTransparent
 * Signature: (J)Z
 */
jboolean JNICALL ULViewIsTransparent_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	return ulViewIsTransparent(reinterpret_cast<ULView>(ViewHandle));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewIsLoading
 * Signature: (J)Z
 */
jboolean JNICALL ULViewIsLoading_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	return ulViewIsLoading(reinterpret_cast<ULView>(ViewHandle));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewGetRenderTarget
 * Signature: (J)Lme/ayydxn/luminescence/gpu/ULRenderTarget;
 */
jobject JNICALL ULViewGetRenderTarget_Native(JNIEnv* Environment, jclass, jlong ViewHandle)
{
	ZoneScoped

	using namespace Luminescence;

	const auto View = reinterpret_cast<ULView>(ViewHandle);
	const auto RenderTarget = ulViewGetRenderTarget(View);

	const jobject RenderTargetObject = Environment->NewObject(CRenderTargetCache::ClassRef, CRenderTargetCache::Constructor);

	Environment->SetBooleanField(RenderTargetObject, CRenderTargetCache::IsEmptyFieldID, RenderTarget.is_empty);
	Environment->SetIntField(RenderTargetObject, CRenderTargetCache::WidthFieldID, static_cast<jint>(RenderTarget.width));
	Environment->SetIntField(RenderTargetObject, CRenderTargetCache::HeightFieldID, static_cast<jint>(RenderTarget.height));
	Environment->SetIntField(RenderTargetObject, CRenderTargetCache::TextureIDField, static_cast<jint>(RenderTarget.texture_id));
	Environment->SetIntField(RenderTargetObject, CRenderTargetCache::TextureWidthField, static_cast<jint>(RenderTarget.texture_width));
	Environment->SetIntField(RenderTargetObject, CRenderTargetCache::TextureHeightField, static_cast<jint>(RenderTarget.texture_height));
	Environment->SetIntField(RenderTargetObject, CRenderTargetCache::RenderBufferFieldID, static_cast<jint>(RenderTarget.render_buffer_id));

	const jobject BitmapFormatEnum = Environment->CallStaticObjectMethod(CBitmapFormatCache::ClassRef, CBitmapFormatCache::FromNativeMethodID,
		static_cast<jint>(RenderTarget.texture_format));
	Environment->SetObjectField(RenderTargetObject, CRenderTargetCache::BitmapFormatFieldID, BitmapFormatEnum);

	const jobject UVRect = Environment->NewObject(CRectCache::ClassRef, CRectCache::ConstructorID, RenderTarget.uv_coords.left, RenderTarget.uv_coords.top,
		RenderTarget.uv_coords.right, RenderTarget.uv_coords.bottom);
	Environment->SetObjectField(RenderTargetObject, CRenderTargetCache::UVCoordsFieldID, UVRect);

	return RenderTargetObject;
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewGetSurface
 * Signature: (J)J
 */
jlong JNICALL ULViewGetSurface_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	const auto View = reinterpret_cast<ULView>(ViewHandle);

	return reinterpret_cast<jlong>(ulViewGetSurface(View));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewLoadHTML
 * Signature: (JLjava/lang/String;)V
 */
void JNICALL ULViewLoadHTML_Native(JNIEnv* Environment, jclass, jlong ViewHandle, jstring HTMLString)
{
	ZoneScoped

	const auto View = reinterpret_cast<ULView>(ViewHandle);
	const auto HTMLStringChars = Environment->GetStringUTFChars(HTMLString, nullptr);
	const auto HTML = ulCreateString(HTMLStringChars);

	ulViewLoadHTML(View, HTML);

	ulDestroyString(HTML);
	Environment->ReleaseStringUTFChars(HTMLString, HTMLStringChars);
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewLoadURL
 * Signature: (JLjava/lang/String;)V
 */
void JNICALL ULViewLoadURL_Native(JNIEnv* Environment, jclass, jlong ViewHandle, jstring URLString)
{
	ZoneScoped

	const auto View = reinterpret_cast<ULView>(ViewHandle);
	const auto URLStringChars = Environment->GetStringUTFChars(URLString, nullptr);
	const auto URL = ulCreateString(URLStringChars);

	ulViewLoadURL(View, URL);

	ulDestroyString(URL);
	Environment->ReleaseStringUTFChars(URLString, URLStringChars);
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewResize
 * Signature: (JII)V
 */
void JNICALL ULViewResize_Native(JNIEnv*, jclass, jlong ViewHandle, jint Width, jint Height)
{
	ZoneScoped

	const auto View = reinterpret_cast<ULView>(ViewHandle);

	ulViewResize(View, static_cast<unsigned int>(Width), static_cast<unsigned int>(Height));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewLockJSContext
 * Signature: (J)J
 */
jlong JNICALL ULViewLockJSContext_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	const auto View = reinterpret_cast<ULView>(ViewHandle);

	return reinterpret_cast<jlong>(ulViewLockJSContext(View));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewUnlockJSContext
 * Signature: (J)V
 */
void JNICALL ULViewUnlockJSContext_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	ulViewUnlockJSContext(reinterpret_cast<ULView>(ViewHandle));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewEvaluateScript
 * Signature: (JLjava/lang/String;[Ljava/lang/String;)Ljava/lang/String;
 */
jstring JNICALL ULViewEvaluateScript_Native(JNIEnv* Environment, jclass, jlong ViewHandle, jstring ScriptString, jobjectArray Exceptions)
{
	ZoneScoped

	const char* ScriptChars = Environment->GetStringUTFChars(ScriptString, nullptr);
	ULString ULScript = ulCreateString(ScriptChars);

	// (Ayydxn) Note to future self: Do NOT destroy this. According to the Ultralight documentation, it's owned by the view.
	ULString ExceptionHandle = nullptr;

	ULString ResultHandle = ulViewEvaluateScript(reinterpret_cast<ULView>(ViewHandle), ULScript, &ExceptionHandle);

	jstring ResultJavaString = nullptr;
	if (ResultHandle && !ulStringIsEmpty(ResultHandle))
		ResultJavaString = Environment->NewStringUTF(ulStringGetData(ResultHandle));

	if (Exceptions && Environment->GetArrayLength(Exceptions) > 0 && ExceptionHandle)
	{
		if (!ulStringIsEmpty(ExceptionHandle))
		{
			jstring ExceptionJavaString = Environment->NewStringUTF(ulStringGetData(ExceptionHandle));

			Environment->SetObjectArrayElement(Exceptions, 0, ExceptionJavaString);
		}
	}

	ulDestroyString(ULScript);
	Environment->ReleaseStringUTFChars(ScriptString, ScriptChars);

	return ResultJavaString;
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewCanGoBack
 * Signature: (J)Z
 */
jboolean JNICALL ULViewCanGoBack_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	return static_cast<jboolean>(ulViewCanGoBack(reinterpret_cast<ULView>(ViewHandle)));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewCanGoForward
 * Signature: (J)Z
 */
jboolean JNICALL ULViewCanGoForward_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	return static_cast<jboolean>(ulViewCanGoForward(reinterpret_cast<ULView>(ViewHandle)));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewGoBack
 * Signature: (J)V
 */
void JNICALL ULViewGoBack_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	ulViewGoBack(reinterpret_cast<ULView>(ViewHandle));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewGoForward
 * Signature: (J)V
 */
void JNICALL ULViewGoForward_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	ulViewGoForward(reinterpret_cast<ULView>(ViewHandle));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewGoToHistoryOffset
 * Signature: (JI)V
 */
void JNICALL ULViewGoToHistoryOffset_Native(JNIEnv*, jclass, jlong ViewHandle, jint Offset)
{
	ZoneScoped

	ulViewGoToHistoryOffset(reinterpret_cast<ULView>(ViewHandle), Offset);
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewReload
 * Signature: (J)V
 */
void JNICALL ULViewReload_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	ulViewReload(reinterpret_cast<ULView>(ViewHandle));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewStop
 * Signature: (J)V
 */
void JNICALL ULViewStop_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	ulViewStop(reinterpret_cast<ULView>(ViewHandle));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewFocus
 * Signature: (J)V
 */
void JNICALL ULViewFocus_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	ulViewFocus(reinterpret_cast<ULView>(ViewHandle));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewUnfocus
 * Signature: (J)V
 */
void JNICALL ULViewUnfocus_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	ulViewUnfocus(reinterpret_cast<ULView>(ViewHandle));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewHasFocus
 * Signature: (J)Z
 */
jboolean JNICALL ULViewHasFocus_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	return static_cast<jboolean>(ulViewHasFocus(reinterpret_cast<ULView>(ViewHandle)));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewHasInputFocus
 * Signature: (J)Z
 */
jboolean JNICALL ULViewHasInputFocus_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ZoneScoped

	return static_cast<jboolean>(ulViewHasInputFocus(reinterpret_cast<ULView>(ViewHandle)));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewFireKeyEvent
 * Signature: (JJ)V
 */
void JNICALL ULViewFireKeyEvent_Native(JNIEnv*, jclass, jlong ViewHandle, jlong KeyEventHandle)
{
	ZoneScoped

	const auto View = reinterpret_cast<ULView>(ViewHandle);
	const auto KeyEvent = reinterpret_cast<ULKeyEvent>(KeyEventHandle);

	ulViewFireKeyEvent(View, KeyEvent);
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewFireMouseEvent
 * Signature: (JJ)V
 */
void JNICALL ULViewFireMouseEvent_Native(JNIEnv*, jclass, jlong ViewHandle, jlong MouseEventHandle)
{
	ZoneScoped

	const auto View = reinterpret_cast<ULView>(ViewHandle);
	const auto MouseEvent = reinterpret_cast<ULMouseEvent>(MouseEventHandle);

	ulViewFireMouseEvent(View, MouseEvent);
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewFireScrollEvent
 * Signature: (JJ)V
 */
void JNICALL ULViewFireScrollEvent_Native(JNIEnv*, jclass, jlong ViewHandle, jlong ScrollEventHandle)
{
	ZoneScoped

	const auto View = reinterpret_cast<ULView>(ViewHandle);
	const auto ScrollEvent = reinterpret_cast<ULScrollEvent>(ScrollEventHandle);

	ulViewFireScrollEvent(View, ScrollEvent);
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewSetViewListener
 * Signature: (JLme/ayydxn/luminescence/view/ULViewListener;)V
 */
void JNICALL ULViewSetViewListener_Native(JNIEnv* Environment, jclass, jlong ViewHandle, jobject ViewListenerInstance)
{
	using namespace Luminescence;

	const auto View = reinterpret_cast<ULView>(ViewHandle);
	const auto Key = reinterpret_cast<uintptr_t>(View);
	auto& CallbackAdapterRegistry = CCallbackAdapterRegistry::GetInstance();

	if (const auto* OldCallbackAdapter = CallbackAdapterRegistry.Find(Key))
	{
		CallbackAdapterRegistry.Unregister(Key);

		delete OldCallbackAdapter;
	}

	if (ViewListenerInstance)
	{
		CallbackAdapterRegistry.Register(Key, new CViewCallbackAdapter(Environment, ViewListenerInstance));

		ulViewSetChangeTitleCallback(View, OnChangeTitle_Trampoline, nullptr);
		ulViewSetChangeURLCallback(View, OnChangeURL_Trampoline, nullptr);
		ulViewSetChangeTooltipCallback(View, OnTooltipChange_Trampoline, nullptr);
		ulViewSetChangeCursorCallback(View, OnCursorChange_Trampoline, nullptr);
		ulViewSetAddConsoleMessageCallback(View, OnConsoleMessageAdded_Trampoline, nullptr);
		ulViewSetCreateChildViewCallback(View, OnChildViewCreated_Trampoline, nullptr);
		ulViewSetCreateInspectorViewCallback(View, OnInspectorViewCreated_Trampoline, nullptr);
		ulViewSetBeginLoadingCallback(View, OnLoadingBegin_Trampoline, nullptr);
		ulViewSetFinishLoadingCallback(View, OnLoadingFinish_Trampoline, nullptr);
		ulViewSetFailLoadingCallback(View, OnLoadingFail_Trampoline, nullptr);
		ulViewSetWindowObjectReadyCallback(View, OnWindowObjectReady_Trampoline, nullptr);
		ulViewSetDOMReadyCallback(View, OnDOMReady_Trampoline, nullptr);
		ulViewSetUpdateHistoryCallback(View, OnUpdateHistory_Trampoline, nullptr);
	}
	else
	{
		ulViewSetChangeTitleCallback(View, nullptr, nullptr);
		ulViewSetChangeURLCallback(View, nullptr, nullptr);
		ulViewSetChangeTooltipCallback(View, nullptr, nullptr);
		ulViewSetChangeCursorCallback(View, nullptr, nullptr);
		ulViewSetAddConsoleMessageCallback(View, nullptr, nullptr);
		ulViewSetCreateChildViewCallback(View, nullptr, nullptr);
		ulViewSetCreateInspectorViewCallback(View, nullptr, nullptr);
		ulViewSetBeginLoadingCallback(View, nullptr, nullptr);
		ulViewSetFinishLoadingCallback(View, nullptr, nullptr);
		ulViewSetFailLoadingCallback(View, nullptr, nullptr);
		ulViewSetWindowObjectReadyCallback(View, nullptr, nullptr);
		ulViewSetDOMReadyCallback(View, nullptr, nullptr);
		ulViewSetUpdateHistoryCallback(View, nullptr, nullptr);
	}
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewSetNeedsPaint
 * Signature: (JZ)V
 */
void JNICALL ULViewSetNeedsPaint_Native(JNIEnv*, jclass, jlong ViewHandle, jboolean bNeedsPaint)
{
	const auto View = reinterpret_cast<ULView>(ViewHandle);

	ulViewSetNeedsPaint(View, static_cast<bool>(bNeedsPaint));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewGetNeedsPaint
 * Signature: (J)Z
 */
jboolean JNICALL ULViewGetNeedsPaint_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	return ulViewGetNeedsPaint(reinterpret_cast<ULView>(ViewHandle));
}

/*
 * Class:     me_ayydxn_luminescence_view_ULView_NativeMethods
 * Method:    nulViewCreateLocalInspectorView
 * Signature: (J)V
 */
void JNICALL ULViewCreateLocalInspectorView_Native(JNIEnv*, jclass, jlong ViewHandle)
{
	ulViewCreateLocalInspectorView(reinterpret_cast<ULView>(ViewHandle));
}

static constexpr JNINativeMethod ViewMethods[] = {
	JNI_METHOD("nulCreateView", "(JIIJJ)J", ULCreateView_Native),
	JNI_METHOD("nulDestroyView", "(J)V", ULDestroyView_Native),
	JNI_METHOD("nulViewGetURL", "(J)Ljava/lang/String;", ULViewGetURL_Native),
	JNI_METHOD("nulViewGetTitle", "(J)Ljava/lang/String;", ULViewGetTitle_Native),
	JNI_METHOD("nulViewGetWidth", "(J)I", ULViewGetWidth_Native),
	JNI_METHOD("nulViewGetHeight", "(J)I", ULViewGetHeight_Native),
	JNI_METHOD("nulViewGetDisplayId", "(J)I", ULViewGetDisplayID_Native),
	JNI_METHOD("nulViewSetDisplayId", "(JI)V", ULViewSetDisplayID_Native),
	JNI_METHOD("nulViewGetDeviceScale", "(J)D", ULViewGetDeviceScale_Native),
	JNI_METHOD("nulViewSetDeviceScale", "(JD)V", ULViewSetDeviceScale_Native),
	JNI_METHOD("nulViewIsAccelerated", "(J)Z", ULViewIsAccelerated_Native),
	JNI_METHOD("nulViewIsTransparent", "(J)Z", ULViewIsTransparent_Native),
	JNI_METHOD("nulViewIsLoading", "(J)Z", ULViewIsLoading_Native),
	JNI_METHOD("nulViewGetRenderTarget", "(J)Lme/ayydxn/luminescence/gpu/ULRenderTarget;", ULViewGetRenderTarget_Native),
	JNI_METHOD("nulViewGetSurface", "(J)J", ULViewGetSurface_Native),
	JNI_METHOD("nulViewLoadHTML", "(JLjava/lang/String;)V", ULViewLoadHTML_Native),
	JNI_METHOD("nulViewLoadURL", "(JLjava/lang/String;)V", ULViewLoadURL_Native),
	JNI_METHOD("nulViewResize", "(JII)V", ULViewResize_Native),
	JNI_METHOD("nulViewLockJSContext", "(J)J", ULViewLockJSContext_Native),
	JNI_METHOD("nulViewUnlockJSContext", "(J)V", ULViewUnlockJSContext_Native),
	JNI_METHOD("nulViewEvaluateScript", "(JLjava/lang/String;[Ljava/lang/String;)Ljava/lang/String;", ULViewEvaluateScript_Native),
	JNI_METHOD("nulViewCanGoBack", "(J)Z", ULViewCanGoBack_Native),
	JNI_METHOD("nulViewCanGoForward", "(J)Z", ULViewCanGoForward_Native),
	JNI_METHOD("nulViewGoBack", "(J)V", ULViewGoBack_Native),
	JNI_METHOD("nulViewGoForward", "(J)V", ULViewGoForward_Native),
	JNI_METHOD("nulViewGoToHistoryOffset", "(JI)V", ULViewGoToHistoryOffset_Native),
	JNI_METHOD("nulViewReload", "(J)V", ULViewReload_Native),
	JNI_METHOD("nulViewStop", "(J)V", ULViewStop_Native),
	JNI_METHOD("nulViewFocus", "(J)V", ULViewFocus_Native),
	JNI_METHOD("nulViewUnfocus", "(J)V", ULViewUnfocus_Native),
	JNI_METHOD("nulViewHasFocus", "(J)Z", ULViewHasFocus_Native),
	JNI_METHOD("nulViewHasInputFocus", "(J)Z", ULViewHasInputFocus_Native),
	JNI_METHOD("nulViewFireKeyEvent", "(JJ)V", ULViewFireKeyEvent_Native),
	JNI_METHOD("nulViewFireMouseEvent", "(JJ)V", ULViewFireMouseEvent_Native),
	JNI_METHOD("nulViewFireScrollEvent", "(JJ)V", ULViewFireScrollEvent_Native),
	JNI_METHOD("nulViewSetViewListener", "(JLme/ayydxn/luminescence/view/ULViewListener;)V", ULViewSetViewListener_Native),
	JNI_METHOD("nulViewSetNeedsPaint", "(JZ)V", ULViewSetNeedsPaint_Native),
	JNI_METHOD("nulViewGetNeedsPaint", "(J)Z", ULViewGetNeedsPaint_Native),
	JNI_METHOD("nulViewCreateLocalInspectorView", "(J)V", ULViewCreateLocalInspectorView_Native),
};

bool Luminescence::RegisterViewMethods(JNIEnv* Environment)
{
	const bool bCacheInitializationStatus = CRenderTargetCache::InitializeCache(Environment) && CViewCache::InitializeCache(Environment);
	const bool bMethodRegistrationStatus = RegisterNativeMethods(Environment, "me/ayydxn/luminescence/view/ULView$NativeMethods",
		JNI_METHODS_AND_COUNT(ViewMethods));
	
	return bCacheInitializationStatus && bMethodRegistrationStatus;
}