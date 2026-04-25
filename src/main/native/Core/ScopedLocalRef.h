//
// Created by Ayydxn on 4/8/2026.
//

#pragma once

#include <jni.h>

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
