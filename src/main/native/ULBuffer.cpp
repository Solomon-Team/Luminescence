//
// Created by Ayydxn on 4/9/2026.
//

#include "Core/JNIUtilities.h"

#include <Ultralight/CAPI/CAPI_Buffer.h>

#include <vector>

/*
 * Class:     me_ayydxn_luminescence_util_ULBuffer_NativeMethods
 * Method:    nulCreateBufferFromDirect
 * Signature: (Ljava/nio/ByteBuffer;I)J
 */
jlong JNICALL ULCreateBufferFromDirect_Native(JNIEnv* Environment, jclass, jobject DirectByteBuffer, jint Capacity)
{
    void* Data = Environment->GetDirectBufferAddress(DirectByteBuffer);
    if (!Data)
        return 0; // Not a direct buffer or capacity is 0

    // Destruction callback is a no-op — Java owns this memory.
    ULBuffer Buffer = ulCreateBuffer(Data, static_cast<size_t>(Capacity), nullptr, [](void*, void*) {});

    return reinterpret_cast<jlong>(Buffer);
}

/*
 * Class:     me_ayydxn_luminescence_util_ULBuffer_NativeMethods
 * Method:    nulCreateBufferFromCopy
 * Signature: ([B)J
 */
jlong JNICALL ULCreateBufferFromCopy_Native(JNIEnv* Environment, jclass, jbyteArray Data)
{
    const auto Length = Environment->GetArrayLength(Data);
    
    // Allocate a temporary buffer for the copy
    std::vector<jbyte> Buffer(Length);
    Environment->GetByteArrayRegion(Data, 0, Length, Buffer.data());
    
    // Ultralight then makes its own internal copy from this buffer
    ULBuffer Result = ulCreateBufferFromCopy(Buffer.data(), static_cast<size_t>(Length));

    return reinterpret_cast<jlong>(Result);
}

/*
 * Class:     me_ayydxn_luminescence_util_ULBuffer_NativeMethods
 * Method:    nulDestroyBuffer
 * Signature: (J)V
 */
void JNICALL ULDestroyBuffer_Native(JNIEnv*, jclass, jlong BufferHandle)
{
    ulDestroyBuffer(reinterpret_cast<ULBuffer>(BufferHandle));
}

/*
 * Class:     me_ayydxn_luminescence_util_ULBuffer_NativeMethods
 * Method:    nulBufferGetSize
 * Signature: (J)J
 */
jlong JNICALL ULBufferGetSize_Native(JNIEnv*, jclass, jlong BufferHandle)
{
    const auto Buffer = reinterpret_cast<ULBuffer>(BufferHandle);
    
    return static_cast<jlong>(ulBufferGetSize(Buffer));
}

/*
 * Class:     me_ayydxn_luminescence_util_ULBuffer_NativeMethods
 * Method:    nulBufferOwnsData
 * Signature: (J)Z
 */
jboolean JNICALL ULBufferOwnsData_Native(JNIEnv*, jclass, jlong BufferHandle)
{
    const auto Buffer = reinterpret_cast<ULBuffer>(BufferHandle);
    
    return ulBufferOwnsData(Buffer);
}

/*
 * Class:     me_ayydxn_luminescence_util_ULBuffer_NativeMethods
 * Method:    nulBufferToByteArray
 * Signature: (J)[B
 */
jbyteArray JNICALL ULBufferToByteArray_Native(JNIEnv* Environment, jclass, jlong BufferHandle)
{
    const auto Buffer = reinterpret_cast<ULBuffer>(BufferHandle);
    const auto* Data = static_cast<const jbyte*>(ulBufferGetData(Buffer));
    const auto Size = static_cast<jsize>(ulBufferGetSize(Buffer));

    const jbyteArray Result = Environment->NewByteArray(Size);
    if (!Result)
        return nullptr;

    Environment->SetByteArrayRegion(Result, 0, Size, Data);

    return Result;
}

static constexpr JNINativeMethod BufferMethods[] =
{
    JNI_METHOD("nulCreateBufferFromDirect", "(Ljava/nio/ByteBuffer;I)J", ULCreateBufferFromDirect_Native),
    JNI_METHOD("nulCreateBufferFromCopy", "([B)J", ULCreateBufferFromCopy_Native),
    JNI_METHOD("nulDestroyBuffer", "(J)V", ULDestroyBuffer_Native),
    JNI_METHOD("nulBufferGetSize", "(J)J", ULBufferGetSize_Native),
    JNI_METHOD("nulBufferOwnsData", "(J)Z", ULBufferOwnsData_Native),
    JNI_METHOD("nulBufferToByteArray", "(J)[B", ULBufferToByteArray_Native),
};

bool Luminescence::RegisterBufferMethods(JNIEnv* Environment)
{
    return RegisterNativeMethods(Environment, "me/ayydxn/luminescence/util/ULBuffer$NativeMethods", JNI_METHODS_AND_COUNT(BufferMethods));
}
