#pragma once

#include "Core/CallbackAdapter.h"

#include <Ultralight/CAPI/CAPI_String.h>

namespace Luminescence
{
	class CViewCallbackAdapter : public ICallbackAdapter
	{
	public:
		CViewCallbackAdapter(JNIEnv* Environment, jobject ViewListenerInstance);

		void OnChangeTitle(ULString Title) const;
		void OnChangeURL(ULString URL) const;
		void OnTooltipChange(ULString URL) const;
		void OnCursorChange(ULCursor Cursor) const;
		void OnConsoleMessageAdded(ULMessageSource Source, ULMessageLevel Level, ULString Message, unsigned int LineNumber, unsigned int ColumnNumber, ULString SourceID) const;
		ULView OnChildViewCreated(ULString OpenerURL, ULString TargetURL, bool bIsPopup, ULIntRect PopupRect) const;
		ULView OnInspectorViewCreated(bool bIsLocal, ULString InspectedURL) const;
		void OnLoadingBegin(unsigned long long FrameID, bool bIsMainFrame, ULString URL) const;
		void OnLoadingFinish(unsigned long long FrameID, bool bIsMainFrame, ULString URL) const;
		void OnLoadingFail(unsigned long long FrameID, bool bIsMainFrame, ULString URL, ULString Description, ULString ErrorDomain, int ErrorCode) const;
		void OnWindowObjectReady(unsigned long long FrameID, bool bIsMainFrame, ULString URL) const;
		void OnDOMReady(unsigned long long FrameID, bool bIsMainFrame, ULString URL) const;
		void OnUpdateHistory() const;
	};
}