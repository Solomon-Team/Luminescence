#pragma once

#include "Core/CallbackAdapter.h"

#include <Ultralight/CAPI/CAPI_GPUDriver.h>

namespace Luminescence
{
    class CGPUDriverCallbackAdapter : public ICallbackAdapter
    {
    public:
        CGPUDriverCallbackAdapter(JNIEnv* Environment, jobject JavaImplementationInstance);
        ~CGPUDriverCallbackAdapter() override;

        ULGPUDriver MakeStruct();

    private:
        static void BeginSynchronize_Trampoline();
        static void EndSynchronize_Trampoline();
        static unsigned int NextTextureID_Trampoline();
        static void CreateTexture_Trampoline(unsigned int TextureID, ULBitmap Bitmap);
        static void UpdateTexture_Trampoline(unsigned int TextureID, ULBitmap Bitmap);
        static void DestroyTexture_Trampoline(unsigned int TextureID);
        static unsigned int NextRenderBufferID_Trampoline();
        static void CreateRenderBuffer_Trampoline(unsigned int RenderBufferID, ULRenderBuffer RenderBuffer);
        static void DestroyRenderBuffer_Trampoline(unsigned int RenderBufferID);
        static unsigned int NextGeometryID_Trampoline();
        static void CreateGeometry_Trampoline(unsigned int GeometryID, ULVertexBuffer Vertices, ULIndexBuffer Indices);
        static void UpdateGeometry_Trampoline(unsigned int GeometryID, ULVertexBuffer Vertices, ULIndexBuffer Indices);
        static void DestroyGeometry_Trampoline(unsigned int GeometryID);
        static void UpdateCommandList_Trampoline(ULCommandList CommandList);

        /* -- Helpers -- */
        static jbyteArray BytesToJavaArray(JNIEnv* Environment, const void* Data, size_t Size);
        static jobject BuildGPUState(JNIEnv* Environment, const CGPUDriverCallbackAdapter* Self, const ULGPUState& GPUState);
        static jobject BuildVertexBuffer(JNIEnv* Environment, const CGPUDriverCallbackAdapter* Self, const ULVertexBuffer& VertexBuffer);
        static jobject BuildIndexBuffer(JNIEnv* Environment, const CGPUDriverCallbackAdapter* Self, const ULIndexBuffer& IndexBuffer);
    private:
        inline static thread_local CGPUDriverCallbackAdapter* m_ActiveCallbackAdapter = nullptr;

        jmethodID m_BeginSynchronizeMethodID = nullptr;
        jmethodID m_EndSynchronizeMethodID = nullptr;
        jmethodID m_NextTextureIdMethodID = nullptr;
        jmethodID m_CreateTextureMethodID = nullptr;
        jmethodID m_UpdateTextureMethodID = nullptr;
        jmethodID m_DestroyTextureMethodID = nullptr;
        jmethodID m_NextRenderBufferIdMethodID = nullptr;
        jmethodID m_CreateRenderBufferMethodID = nullptr;
        jmethodID m_DestroyRenderBufferMethodID = nullptr;
        jmethodID m_NextGeometryIdMethodID = nullptr;
        jmethodID m_CreateGeometryMethodID = nullptr;
        jmethodID m_UpdateGeometryMethodID = nullptr;
        jmethodID m_DestroyGeometryMethodID = nullptr;
        jmethodID m_UpdateCommandListMethodID = nullptr;

        // Cached class/constructor references for building Java objects
        jclass m_CommandClass = nullptr;
        jmethodID m_CommandConstructorID = nullptr;
        jclass m_ShaderTypeEnumClass = nullptr;
        jmethodID m_ShaderTypeFromNativeMethodID = nullptr;
        jclass m_GPUStateClass = nullptr;
        jmethodID m_GPUStateConstructorID = nullptr;
        jfieldID m_GPUStateViewportWidth = nullptr;
        jfieldID m_GPUStateViewportHeight = nullptr;
        jfieldID m_GPUStateEnableTexturingField = nullptr;
        jfieldID m_GPUStateEnableBlendField = nullptr;
        jfieldID m_GPUStateRenderBufferIDField = nullptr;
        jfieldID m_GPUStateTexture1IDField = nullptr;
        jfieldID m_GPUStateTexture2IDField = nullptr;
        jfieldID m_GPUStateTexture3IDField = nullptr;
        jfieldID m_GPUStateClipSizeField = nullptr;
        jfieldID m_GPUStateEnableScissorField = nullptr;
        jfieldID m_GPUStateShaderTypeFieldID = nullptr;
        jclass m_RenderBufferClass = nullptr;
        jmethodID m_RenderBufferConstructorID = nullptr;
        jclass m_VertexBufferClass = nullptr;
        jmethodID m_VertexBufferConstructorID = nullptr;
        jclass m_VertexFormatClass = nullptr;
        jmethodID m_VertexFormatFromNativeID = nullptr;
        jclass m_IndexBufferClass = nullptr;
        jmethodID m_IndexBufferConstructorID = nullptr;
        jclass m_IntRectClass = nullptr;
        jmethodID m_IntRectConstructorID = nullptr;
    };
}
