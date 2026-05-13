//
// Created by Ayydxn on 4/6/2026.
//

#include "Core/JNIUtilities.h"
#include "Core/Profiling.h"

#include <Ultralight/CAPI/CAPI_String.h>

#include <codecvt>
#include <exception>
#include <locale>
#include <string>

/*
 * Class:     me_ayydxn_luminescence_util_ULString_NativeMethods
 * Method:    nulCreateString
 * Signature: (Ljava/lang/String;)J
 */
jlong JNICALL ULCreateString_Native(JNIEnv* Environment, jclass, jstring String)
{
    ZoneScoped
    
    const char* StringCStr = Environment->GetStringUTFChars(String, nullptr);
    
    ULString Result = ulCreateString(StringCStr);
    
    Environment->ReleaseStringUTFChars(String, StringCStr);
    
    return reinterpret_cast<jlong>(Result);
}

/*
 * Class:     me_ayydxn_luminescence_util_ULString_NativeMethods
 * Method:    nulCreateStringUTF8
 * Signature: (Ljava/lang/String;)J
 */
jlong JNICALL ULCreateStringUTF8_Native(JNIEnv* Environment, jclass, jstring UTF8String)
{
    ZoneScoped
    
    const char* StringCStr = Environment->GetStringUTFChars(UTF8String, nullptr);
    const size_t StringLength = Environment->GetStringUTFLength(UTF8String);
    
    ULString Result = ulCreateStringUTF8(StringCStr, StringLength);
    
    Environment->ReleaseStringUTFChars(UTF8String, StringCStr);
    
    return reinterpret_cast<jlong>(Result);
}

/*
 * Class:     me_ayydxn_luminescence_util_ULString_NativeMethods
 * Method:    nulCreateStringUTF16
 * Signature: (Ljava/lang/String;)J
 */
jlong JNICALL ULCreateStringUTF16_Native(JNIEnv* Environment, jclass, jstring UTF16String)
{
    ZoneScoped
    
    const jchar* StringChars = Environment->GetStringChars(UTF16String, nullptr);
    const jsize Length = Environment->GetStringLength(UTF16String);

    ULString Result = ulCreateStringUTF16((ULChar16*) StringChars, static_cast<size_t>(Length));
    
    Environment->ReleaseStringChars(UTF16String, StringChars);

    return reinterpret_cast<jlong>(Result);
}

/*
 * Class:     me_ayydxn_luminescence_util_ULString_NativeMethods
 * Method:    nulCreateStringFromCopy
 * Signature: (J)J
 */
jlong JNICALL ULCreateStringFromCopy_Native(JNIEnv*, jclass, jlong OtherStringHandle)
{
    ZoneScoped
    
    const auto OtherString = reinterpret_cast<ULString>(OtherStringHandle);
    
    return reinterpret_cast<jlong>(ulCreateStringFromCopy(OtherString));
}

/*
 * Class:     me_ayydxn_luminescence_util_ULString_NativeMethods
 * Method:    nulDestroyString
 * Signature: (J)V
 */
void JNICALL ULDestroyString_Native(JNIEnv*, jclass, jlong StringHandle)
{
    ZoneScoped
    
    ulDestroyString(reinterpret_cast<ULString>(StringHandle));
}

/*
 * Class:     me_ayydxn_luminescence_util_ULString_NativeMethods
 * Method:    nulStringGetData
 * Signature: (J)Ljava/lang/String;
 */
jstring JNICALL ULStringGetData_Native(JNIEnv* Environment, jclass, jlong StringHandle)
{
    ZoneScoped
    
    const auto String = reinterpret_cast<ULString>(StringHandle);
    if (!String)
        return nullptr;

    const char* Data = ulStringGetData(String);
    if (!Data || strlen(Data) == 0)
        return Environment->NewStringUTF("");

    try
    {
        // Convert Standard UTF-8 (Ultralight) to UTF-16 (Java)
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> Convert;
        std::u16string UTF16String = Convert.from_bytes(Data);

        // Pass the UTF-16 buffer directly to Java
        // NewString takes a jchar* (16-bit) and the count of units
        return Environment->NewString(reinterpret_cast<const jchar*>(UTF16String.data()), static_cast<jsize>(UTF16String.length()));
    } 
    catch (const std::exception&)
    {
        // Fallback for conversion errors (though rare with valid UTF-8)
        return Environment->NewStringUTF(Data);
    }
}

/*
 * Class:     me_ayydxn_luminescence_util_ULString_NativeMethods
 * Method:    nulStringGetLength
 * Signature: (J)J
 */
jlong JNICALL ULStringGetLength_Native(JNIEnv*, jclass, jlong StringHandle)
{
    ZoneScoped
    
    const auto String = reinterpret_cast<ULString>(StringHandle);
    
    return static_cast<jlong>(ulStringGetLength(String));
}

/*
 * Class:     me_ayydxn_luminescence_util_ULString_NativeMethods
 * Method:    nulStringIsEmpty
 * Signature: (J)Z
 */
jboolean JNICALL ULStringIsEmpty_Native(JNIEnv *, jclass, jlong StringHandle)
{
    ZoneScoped
    
    const auto String = reinterpret_cast<ULString>(StringHandle);
    
    return ulStringIsEmpty(String);
}

/*
 * Class:     me_ayydxn_luminescence_util_ULString_NativeMethods
 * Method:    nulStringAssignString
 * Signature: (JJ)V
 */
void JNICALL ULStringAssignString_Native(JNIEnv*, jclass, jlong StringHandle, jlong StringToAssign)
{
    ZoneScoped
    
    const auto String = reinterpret_cast<ULString>(StringHandle);
    const auto NewString = reinterpret_cast<ULString>(StringToAssign);
    
    ulStringAssignString(String, NewString);
}

/*
 * Class:     me_ayydxn_luminescence_util_ULString_NativeMethods
 * Method:    nulAssignCString
 * Signature: (JLjava/lang/String;)V
 */
void JNICALL ULAssignCString_Native(JNIEnv* Environment, jclass, jlong StringHandle, jstring String)
{
    ZoneScoped
    
    if (String == nullptr)
        return;

    const char* StringCStr = Environment->GetStringUTFChars(String, nullptr);
    ulStringAssignCString(reinterpret_cast<ULString>(StringHandle), StringCStr);
    
    Environment->ReleaseStringUTFChars(String, StringCStr);
}

static constexpr JNINativeMethod StringMethods[] =
{
    JNI_METHOD("nulCreateString", "(Ljava/lang/String;)J", ULCreateString_Native),
    JNI_METHOD("nulCreateStringUTF8", "(Ljava/lang/String;)J", ULCreateStringUTF8_Native),
    JNI_METHOD("nulCreateStringUTF16", "(Ljava/lang/String;)J", ULCreateStringUTF16_Native),
    JNI_METHOD("nulCreateStringFromCopy", "(J)J", ULCreateStringFromCopy_Native),
    JNI_METHOD("nulDestroyString", "(J)V", ULDestroyString_Native),
    JNI_METHOD("nulStringGetData", "(J)Ljava/lang/String;", ULStringGetData_Native),
    JNI_METHOD("nulStringGetLength", "(J)J", ULStringGetLength_Native),
    JNI_METHOD("nulStringIsEmpty", "(J)Z", ULStringIsEmpty_Native),
    JNI_METHOD("nulStringAssignString", "(JJ)V", ULStringAssignString_Native),
    JNI_METHOD("nulAssignCString", "(JLjava/lang/String;)V", ULAssignCString_Native),
};

bool Luminescence::RegisterStringMethods(JNIEnv* Environment)
{
    return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/util/ULString$NativeMethods", JNI_METHODS_AND_COUNT(StringMethods));
}