//
// Created by Ayydxn on 4/8/2026.
//

#pragma once

// Forward declaration. This is most likely gonna get included in a file where jni.h is being included, so no point in including it here. 
typedef JNIEnv_ JNIEnv;

namespace Luminescence
{
	template<typename T>
	class CScopedLocalRef
	{
	public:
		CScopedLocalRef(JNIEnv* Environment, T Ref)
			: m_Environment(Environment), m_Ref(Ref) {}
		CScopedLocalRef(const CScopedLocalRef&) = delete;

		~CScopedLocalRef()
		{
			if (m_Ref)
				m_Environment->DeleteLocalRef(m_Ref);
		}

		T Get() const { return m_Ref; }
		
		operator T() const { return m_Ref; }
	private:
		JNIEnv* m_Environment;
		T m_Ref;
	};

}
