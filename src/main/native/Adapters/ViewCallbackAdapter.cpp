#include "ViewCallbackAdapter.h"
#include "Cache/IntRectCache.h"
#include "Cache/ViewCache.h"
#include "Core/ScopedLocalRef.h"

namespace Luminescence
{
	CViewCallbackAdapter::CViewCallbackAdapter(JNIEnv* Environment, jobject ViewListenerInstance)
		: ICallbackAdapter(Environment, ViewListenerInstance) {}

	void CViewCallbackAdapter::OnChangeTitle(ULString Title) const
	{
		JNIEnv* Environment = GetJNIEnvironment();
		if (!Environment || !m_JavaImplementation)
			return;

		const CScopedLocalRef TitleJavaString(Environment, Environment->NewStringUTF(ulStringGetData(Title)));
		Environment->CallVoidMethod(m_JavaImplementation, CViewCache::OnTitleChangeMethodID, TitleJavaString.Get());
	}

	void CViewCallbackAdapter::OnChangeURL(ULString URL) const
	{
		JNIEnv* Environment = GetJNIEnvironment();
		if (!Environment || !m_JavaImplementation)
			return;

		const CScopedLocalRef URLJavaString(Environment, Environment->NewStringUTF(ulStringGetData(URL)));
		Environment->CallVoidMethod(m_JavaImplementation, CViewCache::OnURLChangeMethodID, URLJavaString.Get());
	}

	void CViewCallbackAdapter::OnTooltipChange(ULString URL) const
	{
		JNIEnv* Environment = GetJNIEnvironment();
		if (!Environment || !m_JavaImplementation)
			return;
		
		const CScopedLocalRef TooltipJavaString(Environment, Environment->NewStringUTF(ulStringGetData(URL)));
		Environment->CallVoidMethod(m_JavaImplementation, CViewCache::OnTooltipChangeMethodID, TooltipJavaString.Get());
	}

	void CViewCallbackAdapter::OnCursorChange(ULCursor Cursor) const
	{
		JNIEnv* Environment = GetJNIEnvironment();
		if (!Environment || !m_JavaImplementation)
			return;

		const CScopedLocalRef CursorObject(Environment, Environment->CallStaticObjectMethod(CViewCache::CursorClassRef, CViewCache::CursorFromNativeMethodID, static_cast<jint>(Cursor)));
		
		Environment->CallVoidMethod(m_JavaImplementation, CViewCache::OnCursorChangeMethodID, CursorObject.Get());
	}

	void CViewCallbackAdapter::OnConsoleMessageAdded(ULMessageSource Source, ULMessageLevel Level, ULString Message, unsigned int LineNumber, unsigned int ColumnNumber, ULString SourceID) const
	{
		JNIEnv* Environment = GetJNIEnvironment();
		if (!Environment || !m_JavaImplementation)
			return;

		const CScopedLocalRef MessageSourceObject(Environment, Environment->CallStaticObjectMethod(CViewCache::MessageSourceClassRef,
			CViewCache::MessageSourceFromNativeMethodID, static_cast<jint>(Source)));

		const CScopedLocalRef MessageLevelObject(Environment, Environment->CallStaticObjectMethod(CViewCache::MessageLevelClassRef, CViewCache::MessageLevelFromNativeMethodID,
			static_cast<jint>(Level)));

		const CScopedLocalRef MessageJavaString(Environment, Environment->NewStringUTF(ulStringGetData(Message)));
		const CScopedLocalRef SourceIDJavaString(Environment, Environment->NewStringUTF(ulStringGetData(SourceID)));

		if (CViewCache::OnConsoleMessageAddedMethodID)
		{
			Environment->CallVoidMethod(m_JavaImplementation, CViewCache::OnConsoleMessageAddedMethodID, MessageSourceObject.Get(), MessageLevelObject.Get(),
				MessageJavaString.Get(), static_cast<jint>(LineNumber),static_cast<jint>(ColumnNumber), SourceIDJavaString.Get());
		}
	}

	ULView CViewCallbackAdapter::OnChildViewCreated(ULString OpenerURL, ULString TargetURL, bool bIsPopup, ULIntRect PopupRect) const
	{
		JNIEnv* Environment = GetJNIEnvironment();
    	if (!Environment || !m_JavaImplementation)
    	    return nullptr;
		
    	const CScopedLocalRef OpenerURLJavaString(Environment, Environment->NewStringUTF(ulStringGetData(OpenerURL)));
    	const CScopedLocalRef TargetURLJavaString(Environment, Environment->NewStringUTF(ulStringGetData(TargetURL)));

    	const CScopedLocalRef PopupRectObject(Environment, Environment->NewObject(CIntRectCache::ClassRef, CIntRectCache::ConstructorID, static_cast<jint>(PopupRect.left),
    		static_cast<jint>(PopupRect.top), static_cast<jint>(PopupRect.right), static_cast<jint>(PopupRect.bottom)));
		
    	const jobject ViewResultObject = Environment->CallObjectMethod(m_JavaImplementation, CViewCache::OnChildViewCreatedMethodID, OpenerURLJavaString.Get(),
    		TargetURLJavaString.Get(), static_cast<jboolean>(bIsPopup), PopupRectObject.Get());

    	const ULView NativeResult = CViewCache::GetNativeHandle(Environment, ViewResultObject);
		    	
		if (ViewResultObject)
    		Environment->DeleteLocalRef(ViewResultObject);
	
    	return NativeResult;
	}

	ULView CViewCallbackAdapter::OnInspectorViewCreated(bool bIsLocal, ULString InspectedURL) const
	{
		JNIEnv* Environment = GetJNIEnvironment();
		if (!Environment || !m_JavaImplementation)
			return nullptr;

		const CScopedLocalRef InspectedURLJavaString(Environment, Environment->NewStringUTF(ulStringGetData(InspectedURL)));
		
		const jobject ViewResultObject = Environment->CallObjectMethod(m_JavaImplementation, CViewCache::OnInspectorViewCreatedMethodID, static_cast<jboolean>(bIsLocal),
			InspectedURLJavaString.Get());

		const ULView NativeResult = CViewCache::GetNativeHandle(Environment, ViewResultObject);

		Environment->DeleteLocalRef(InspectedURLJavaString);
		
		if (ViewResultObject)
			Environment->DeleteLocalRef(ViewResultObject);

		return NativeResult;
	}

	void CViewCallbackAdapter::OnLoadingBegin(unsigned long long FrameID, bool bIsMainFrame, ULString URL) const
	{
		JNIEnv* Environment = GetJNIEnvironment();
		if (!Environment || !m_JavaImplementation)
			return;
		
		const CScopedLocalRef URLJavaString(Environment, Environment->NewStringUTF(ulStringGetData(URL)));
		
		Environment->CallVoidMethod(m_JavaImplementation, CViewCache::OnLoadingBeginMethodID, static_cast<jlong>(FrameID), static_cast<jboolean>(bIsMainFrame),
			URLJavaString.Get());
	}

	void CViewCallbackAdapter::OnLoadingFinish(unsigned long long FrameID, bool bIsMainFrame, ULString URL) const
	{
		JNIEnv* Environment = GetJNIEnvironment();
		if (!Environment || !m_JavaImplementation)
			return;
		
		const CScopedLocalRef URLJavaString(Environment, Environment->NewStringUTF(ulStringGetData(URL)));
		
		Environment->CallVoidMethod(m_JavaImplementation, CViewCache::OnLoadingFinishMethodID, static_cast<jlong>(FrameID), static_cast<jboolean>(bIsMainFrame),
			URLJavaString.Get());
	}

	void CViewCallbackAdapter::OnLoadingFail(unsigned long long FrameID, bool bIsMainFrame, ULString URL, ULString Description, ULString ErrorDomain, int ErrorCode) const
	{
	    JNIEnv* Environment = GetJNIEnvironment();
	    if (!Environment || !m_JavaImplementation)
	        return;
	
	    const CScopedLocalRef URLJavaString(Environment, Environment->NewStringUTF(ulStringGetData(URL)));
	    const CScopedLocalRef DescriptionJavaString(Environment, Environment->NewStringUTF(ulStringGetData(Description)));
	    const CScopedLocalRef ErrorDomainJavaString(Environment, Environment->NewStringUTF(ulStringGetData(ErrorDomain)));
	
	    Environment->CallVoidMethod(m_JavaImplementation, CViewCache::OnLoadingFailMethodID, static_cast<jlong>(FrameID), static_cast<jboolean>(bIsMainFrame),
	        URLJavaString.Get(), DescriptionJavaString.Get(), ErrorDomainJavaString.Get(), static_cast<jint>(ErrorCode));
	}
	
	void CViewCallbackAdapter::OnWindowObjectReady(unsigned long long FrameID, bool bIsMainFrame, ULString URL) const
	{
	    JNIEnv* Environment = GetJNIEnvironment();
	    if (!Environment || !m_JavaImplementation)
	        return;
	
	    const CScopedLocalRef URLJavaString(Environment, Environment->NewStringUTF(ulStringGetData(URL)));
	
	    Environment->CallVoidMethod(m_JavaImplementation, CViewCache::OnWindowObjectReadyMethodID, static_cast<jlong>(FrameID), static_cast<jboolean>(bIsMainFrame),
	        URLJavaString.Get());
	}
	
	void CViewCallbackAdapter::OnDOMReady(unsigned long long FrameID, bool bIsMainFrame, ULString URL) const
	{
	    JNIEnv* Environment = GetJNIEnvironment();
	    if (!Environment || !m_JavaImplementation)
	        return;
	
	    const CScopedLocalRef URLJavaString(Environment, Environment->NewStringUTF(ulStringGetData(URL)));
	
	    Environment->CallVoidMethod(m_JavaImplementation, CViewCache::OnDOMReadyMethodID, static_cast<jlong>(FrameID), static_cast<jboolean>(bIsMainFrame),
	        URLJavaString.Get());
	}
	
	void CViewCallbackAdapter::OnUpdateHistory() const
	{
	    JNIEnv* Environment = GetJNIEnvironment();
	    if (!Environment || !m_JavaImplementation)
	        return;
	
	    Environment->CallVoidMethod(m_JavaImplementation, CViewCache::OnUpdateHistoryMethodID);
	}
}
