#include "GPUDriverCallbackAdapter.h"
#include "Core/CallbackAdapterRegistry.h"
#include "Core/JNIUtilities.h"
#include "Core/ScopedLocalRef.h"

namespace Luminescence
{
    CGPUDriverCallbackAdapter::CGPUDriverCallbackAdapter(JNIEnv* Environment, jobject JavaImplementationInstance)
        : ICallbackAdapter(Environment, JavaImplementationInstance)
    {
        const CScopedLocalRef DriverClass(Environment, Environment->FindClass("me/ayydxn/luminescence/renderer/gpu/ULGPUDriver"));

        m_BeginSynchronizeMethodID = Environment->GetMethodID(DriverClass, "beginSynchronize", "()V");
        m_EndSynchronizeMethodID = Environment->GetMethodID(DriverClass, "endSynchronize", "()V");
        m_NextTextureIdMethodID = Environment->GetMethodID(DriverClass, "nextTextureId", "()I");
        m_CreateTextureMethodID = Environment->GetMethodID(DriverClass, "createTexture", "(ILme/ayydxn/luminescence/bitmap/ULBitmap;)V");
        m_UpdateTextureMethodID = Environment->GetMethodID(DriverClass, "updateTexture", "(ILme/ayydxn/luminescence/bitmap/ULBitmap;)V");
        m_DestroyTextureMethodID = Environment->GetMethodID(DriverClass, "destroyTexture", "(I)V");
        m_NextRenderBufferIdMethodID = Environment->GetMethodID(DriverClass, "nextRenderBufferId", "()I");
        m_CreateRenderBufferMethodID = Environment->GetMethodID(DriverClass, "createRenderBuffer", "(ILme/ayydxn/luminescence/renderer/gpu/ULGPURenderBuffer;)V");
        m_DestroyRenderBufferMethodID = Environment->GetMethodID(DriverClass, "destroyRenderBuffer", "(I)V");
        m_NextGeometryIdMethodID = Environment->GetMethodID(DriverClass, "nextGeometryId", "()I");
        m_CreateGeometryMethodID = Environment->GetMethodID(DriverClass, "createGeometry", "(ILme/ayydxn/luminescence/renderer/gpu/ULVertexBuffer;"
                                                    "Lme/ayydxn/luminescence/renderer/gpu/ULIndexBuffer;)V");
        m_UpdateGeometryMethodID = Environment->GetMethodID(DriverClass, "updateGeometry", "(ILme/ayydxn/luminescence/renderer/gpu/ULVertexBuffer;"
                                                    "Lme/ayydxn/luminescence/renderer/gpu/ULIndexBuffer;)V");
        m_DestroyGeometryMethodID = Environment->GetMethodID(DriverClass, "destroyGeometry", "(I)V");
        m_UpdateCommandListMethodID = Environment->GetMethodID(DriverClass, "updateCommandList", "([Lme/ayydxn/luminescence/renderer/gpu/ULCommand;)V");
        
        /*---------------------------------------------*/
        /* -- Cache references that we'll use later -- */
        /*---------------------------------------------*/

        // Cache construction references for ULRenderBuffer
        {
            const CScopedLocalRef RenderBufferClassRef(Environment, Environment->FindClass("me/ayydxn/luminescence/renderer/gpu/ULRenderBuffer"));
            m_RenderBufferClass = (jclass) Environment->NewGlobalRef(RenderBufferClassRef);
            m_RenderBufferConstructorID = Environment->GetMethodID(RenderBufferClassRef, "<init>", "(IIZZ)V");
        }

        // Cache ULVertexBuffer, ULVertexBuffer.Format, ULIndexBuffer
        {
            const CScopedLocalRef VertexBufferClassRef(Environment, Environment->FindClass("me/ayydxn/luminescence/renderer/gpu/ULVertexBuffer"));
            
            m_VertexBufferClass = (jclass) Environment->NewGlobalRef(VertexBufferClassRef);
            m_VertexBufferConstructorID = Environment->GetMethodID(VertexBufferClassRef, "<init>",
                "(Lme/ayydxn/luminescence/renderer/gpu/ULVertexBuffer$Format;[B)V");

            const CScopedLocalRef FormatClassRef(Environment, Environment->FindClass("me/ayydxn/luminescence/renderer/gpu/ULVertexBuffer$Format"));
            
            m_VertexFormatClass = (jclass) Environment->NewGlobalRef(FormatClassRef);
            m_VertexFormatFromNativeID = Environment->GetStaticMethodID(FormatClassRef, "fromNativeValue",
                "(I)Lme/ayydxn/luminescence/renderer/gpu/ULVertexBuffer$Format;");
        }
        
        {
            const CScopedLocalRef IndexBufferClass(Environment, Environment->FindClass("me/ayydxn/luminescence/renderer/gpu/ULIndexBuffer"));
            
            m_IndexBufferClass = (jclass) Environment->NewGlobalRef(IndexBufferClass);
            m_IndexBufferConstructorID = Environment->GetMethodID(IndexBufferClass, "<init>", "([B)V");
        }
        
        // Cache ULGPUState and ULCommand
        {
            const CScopedLocalRef GPUStateClassRef(Environment, Environment->FindClass("me/ayydxn/luminescence/renderer/gpu/ULGPUState"));
            
            m_GPUStateClass = (jclass) Environment->NewGlobalRef(GPUStateClassRef);
            m_GPUStateConstructorID = Environment->GetMethodID(GPUStateClassRef, "<init>", "()V");
        }
        
        // Cache ULShaderType
        {
            const CScopedLocalRef ShaderClass(Environment, Environment->FindClass("me/ayydxn/luminescence/gpu/ULShaderType"));
            m_ShaderTypeEnumClass = (jclass) Environment->NewGlobalRef(ShaderClass);
            m_ShaderTypeFromNativeMethodID = Environment->GetStaticMethodID(ShaderClass, "fromNativeValue", "(I)Lme/ayydxn/luminescence/gpu/ULShaderType;");
        }

        m_GPUStateShaderTypeFieldID = Environment->GetFieldID(m_GPUStateClass, "shaderType", "Lme/ayydxn/luminescence/gpu/ULShaderType;");
        
        {
            const CScopedLocalRef CommandClassRef(Environment, Environment->FindClass("me/ayydxn/luminescence/renderer/gpu/ULCommand"));
            
            m_CommandClass = (jclass) Environment->NewGlobalRef(CommandClassRef);
            m_CommandConstructorID = Environment->GetMethodID(CommandClassRef, "<init>", "()V");
        }

        // Cache IntRect
        {
            const CScopedLocalRef RectClassRef(Environment, Environment->FindClass("me/ayydxn/luminescence/geometry/IntRect"));
            
            m_IntRectClass = (jclass) Environment->NewGlobalRef(RectClassRef);
            m_IntRectConstructorID = Environment->GetMethodID(RectClassRef, "<init>", "(IIII)V");
        }

        CCallbackAdapterRegistry::GetInstance().Register(reinterpret_cast<uintptr_t>(this), this);
    }

    CGPUDriverCallbackAdapter::~CGPUDriverCallbackAdapter()
    {
        if (JNIEnv* Environment = GetJNIEnvironment())
        {
            Environment->DeleteGlobalRef(m_RenderBufferClass);
            Environment->DeleteGlobalRef(m_VertexBufferClass);
            Environment->DeleteGlobalRef(m_VertexFormatClass);
            Environment->DeleteGlobalRef(m_IndexBufferClass);
            Environment->DeleteGlobalRef(m_GPUStateClass);
            Environment->DeleteGlobalRef(m_CommandClass);
            Environment->DeleteGlobalRef(m_IntRectClass);
            Environment->DeleteGlobalRef(m_ShaderTypeEnumClass);
        }
        
        CCallbackAdapterRegistry::GetInstance().Unregister(reinterpret_cast<uintptr_t>(this));
    }

    ULGPUDriver CGPUDriverCallbackAdapter::MakeStruct()
    {
        m_ActiveCallbackAdapter = this;

        return {
            .begin_synchronize = &CGPUDriverCallbackAdapter::BeginSynchronize_Trampoline,
            .end_synchronize = &CGPUDriverCallbackAdapter::EndSynchronize_Trampoline,
            .next_texture_id = &CGPUDriverCallbackAdapter::NextTextureID_Trampoline,
            .create_texture = &CGPUDriverCallbackAdapter::CreateTexture_Trampoline,
            .update_texture = &CGPUDriverCallbackAdapter::UpdateTexture_Trampoline,
            .destroy_texture = &CGPUDriverCallbackAdapter::DestroyTexture_Trampoline,
            .next_render_buffer_id = &CGPUDriverCallbackAdapter::NextRenderBufferID_Trampoline,
            .create_render_buffer = &CGPUDriverCallbackAdapter::CreateRenderBuffer_Trampoline,
            .destroy_render_buffer = &CGPUDriverCallbackAdapter::DestroyRenderBuffer_Trampoline,
            .next_geometry_id = &CGPUDriverCallbackAdapter::NextGeometryID_Trampoline,
            .create_geometry = &CGPUDriverCallbackAdapter::CreateGeometry_Trampoline,
            .update_geometry = &CGPUDriverCallbackAdapter::UpdateGeometry_Trampoline,
            .destroy_geometry = &CGPUDriverCallbackAdapter::DestroyGeometry_Trampoline,
            .update_command_list = &CGPUDriverCallbackAdapter::UpdateCommandList_Trampoline,
        };
    }

    void CGPUDriverCallbackAdapter::BeginSynchronize_Trampoline()
    {
        const auto* Self = m_ActiveCallbackAdapter;\
        
        JNIEnv* Environment = Self->GetJNIEnvironment();
        Environment->CallVoidMethod(Self->m_JavaImplementation, Self->m_BeginSynchronizeMethodID);
        
        CheckException(Environment);
    }

    void CGPUDriverCallbackAdapter::EndSynchronize_Trampoline()
    {
        const auto* Self = m_ActiveCallbackAdapter;
        
        JNIEnv* Environment = Self->GetJNIEnvironment();
        Environment->CallVoidMethod(Self->m_JavaImplementation, Self->m_EndSynchronizeMethodID);
        
        CheckException(Environment);
    }

    unsigned int CGPUDriverCallbackAdapter::NextTextureID_Trampoline()
    {
        const auto* Self = m_ActiveCallbackAdapter;
        
        JNIEnv* Environment = Self->GetJNIEnvironment();
        const auto Result = static_cast<unsigned int>(Environment->CallIntMethod(Self->m_JavaImplementation, Self->m_NextTextureIdMethodID));
        
        CheckException(Environment);
        
        return Result;
    }

    void CGPUDriverCallbackAdapter::CreateTexture_Trampoline(unsigned int TextureID, ULBitmap Bitmap)
    {
        const auto* Self = m_ActiveCallbackAdapter;
        JNIEnv* Environment = Self->GetJNIEnvironment();
        
        const CScopedLocalRef BitmapClass(Environment, Environment->FindClass("me/ayydxn/luminescence/bitmap/ULBitmap"));
        const jmethodID BitmapConstructorID = Environment->GetMethodID(BitmapClass, "<init>", "(J)V");
        const CScopedLocalRef BitmapObject(Environment, Environment->NewObject(BitmapClass, BitmapConstructorID, reinterpret_cast<jlong>(Bitmap)));

        Environment->CallVoidMethod(Self->m_JavaImplementation, Self->m_CreateTextureMethodID, static_cast<jint>(TextureID), BitmapObject.Get());
        
        CheckException(Environment);
    }

    void CGPUDriverCallbackAdapter::UpdateTexture_Trampoline(unsigned int TextureID, ULBitmap Bitmap)
    {
        const auto* Self = m_ActiveCallbackAdapter;
        JNIEnv* Environment = Self->GetJNIEnvironment();

        const CScopedLocalRef BitmapClass(Environment, Environment->FindClass("me/ayydxn/luminescence/bitmap/ULBitmap"));
        const jmethodID BitmapConstructorID = Environment->GetMethodID(BitmapClass, "<init>", "(J)V");
        const CScopedLocalRef BitmapObj(Environment, Environment->NewObject(BitmapClass, BitmapConstructorID, reinterpret_cast<jlong>(Bitmap)));

        Environment->CallVoidMethod(Self->m_JavaImplementation, Self->m_UpdateTextureMethodID, static_cast<jint>(TextureID), BitmapObj.Get());
        
        CheckException(Environment);
    }

    void CGPUDriverCallbackAdapter::DestroyTexture_Trampoline(unsigned int TextureID)
    {
        const auto* Self = m_ActiveCallbackAdapter;
        
        JNIEnv* Environment = Self->GetJNIEnvironment();
        Environment->CallVoidMethod(Self->m_JavaImplementation, Self->m_DestroyTextureMethodID, static_cast<jint>(TextureID));
        
        CheckException(Environment);
    }

    unsigned int CGPUDriverCallbackAdapter::NextRenderBufferID_Trampoline()
    {
        const auto* Self = m_ActiveCallbackAdapter;
        
        JNIEnv* Environment = Self->GetJNIEnvironment();
        const auto Result = static_cast<unsigned int>(Environment->CallIntMethod(Self->m_JavaImplementation, Self->m_NextRenderBufferIdMethodID));
        
        CheckException(Environment);
        
        return Result;
    }

    void CGPUDriverCallbackAdapter::CreateRenderBuffer_Trampoline(unsigned int RenderBufferID, ULRenderBuffer RenderBuffer)
    {
        const auto* Self = m_ActiveCallbackAdapter;
        JNIEnv* Environment = Self->GetJNIEnvironment();

        const CScopedLocalRef RenderBufferObject(Environment, Environment->NewObject(Self->m_RenderBufferClass, Self->m_RenderBufferConstructorID,
            static_cast<jint>(RenderBuffer.texture_id), static_cast<jint>(RenderBuffer.width), static_cast<jint>(RenderBuffer.height),
            static_cast<jboolean>(RenderBuffer.has_stencil_buffer), static_cast<jboolean>(RenderBuffer.has_depth_buffer)));

        Environment->CallVoidMethod(Self->m_JavaImplementation, Self->m_CreateRenderBufferMethodID, static_cast<jint>(RenderBufferID), RenderBufferObject.Get());
        
        CheckException(Environment);
    }

    void CGPUDriverCallbackAdapter::DestroyRenderBuffer_Trampoline(unsigned int RenderBufferID)
    {
        const auto* Self = m_ActiveCallbackAdapter;
        
        JNIEnv* Environment = Self->GetJNIEnvironment();
        Environment->CallVoidMethod(Self->m_JavaImplementation, Self->m_DestroyRenderBufferMethodID, static_cast<jint>(RenderBufferID));
        
        CheckException(Environment);
    }

    unsigned int CGPUDriverCallbackAdapter::NextGeometryID_Trampoline()
    {
        const auto* Self = m_ActiveCallbackAdapter;
        
        JNIEnv* Environment = Self->GetJNIEnvironment();
        const auto Result = static_cast<unsigned int>(Environment->CallIntMethod(Self->m_JavaImplementation, Self->m_NextGeometryIdMethodID));
        
        CheckException(Environment);
        
        return Result;
    }

    void CGPUDriverCallbackAdapter::CreateGeometry_Trampoline(unsigned int GeometryID, ULVertexBuffer Vertices, ULIndexBuffer Indices)
    {
        const auto* Self = m_ActiveCallbackAdapter;
        JNIEnv* Environment = Self->GetJNIEnvironment();

        const CScopedLocalRef VertexBufferObject(Environment, BuildVertexBuffer(Environment, Self, Vertices));
        const CScopedLocalRef IndexBufferObject(Environment, BuildIndexBuffer(Environment, Self, Indices));

        Environment->CallVoidMethod(Self->m_JavaImplementation, Self->m_CreateGeometryMethodID, static_cast<jint>(GeometryID), VertexBufferObject.Get(),
            IndexBufferObject.Get());
        
        CheckException(Environment);
    }

    void CGPUDriverCallbackAdapter::UpdateGeometry_Trampoline(unsigned int GeometryID, ULVertexBuffer Vertices, ULIndexBuffer Indices)
    {
        const auto* Self = m_ActiveCallbackAdapter;
        JNIEnv* Environment = Self->GetJNIEnvironment();

        const CScopedLocalRef VertexBufferObject(Environment, BuildVertexBuffer(Environment, Self, Vertices));
        const CScopedLocalRef IndexBufferObject(Environment, BuildIndexBuffer(Environment, Self, Indices));

        Environment->CallVoidMethod(Self->m_JavaImplementation, Self->m_UpdateGeometryMethodID, static_cast<jint>(GeometryID), VertexBufferObject.Get(),
            IndexBufferObject.Get());
        
        CheckException(Environment);
    }

    void CGPUDriverCallbackAdapter::DestroyGeometry_Trampoline(unsigned int GeometryID)
    {
        const auto* Self = m_ActiveCallbackAdapter;
        
        JNIEnv* Environment = Self->GetJNIEnvironment();
        Environment->CallVoidMethod(Self->m_JavaImplementation, Self->m_DestroyGeometryMethodID, static_cast<jint>(GeometryID));
        
        CheckException(Environment);
    }

    void CGPUDriverCallbackAdapter::UpdateCommandList_Trampoline(ULCommandList CommandList)
    {
        const auto* Self = m_ActiveCallbackAdapter;
        JNIEnv* Environment = Self->GetJNIEnvironment();
    
        // Retrieve field/method IDs for ULCommand once per call via the cached class.
        const jfieldID TypeFieldID = Environment->GetFieldID(Self->m_CommandClass, "type", "Lme/ayydxn/luminescence/renderer/gpu/ULCommand$Type;");
        const jfieldID StateFieldID = Environment->GetFieldID(Self->m_CommandClass, "gpuState", "Lme/ayydxn/luminescence/renderer/gpu/ULGPUState;");
        const jfieldID GeometryIDFieldID = Environment->GetFieldID(Self->m_CommandClass, "geometryId",    "I");
        const jfieldID IndicesCountFieldID = Environment->GetFieldID(Self->m_CommandClass, "indicesCount",  "I");
        const jfieldID IndicesOffsetFieldID = Environment->GetFieldID(Self->m_CommandClass, "indicesOffset", "I");
    
        const CScopedLocalRef TypeClass(Environment, Environment->FindClass("me/ayydxn/luminescence/renderer/gpu/ULCommand$Type"));
        const jmethodID TypeValuesID = Environment->GetStaticMethodID(TypeClass, "values", "()[Lme/ayydxn/luminescence/renderer/gpu/ULCommand$Type;");
        const CScopedLocalRef TypeValues(Environment, (jobjectArray) Environment->CallStaticObjectMethod(TypeClass, TypeValuesID));
    
        const CScopedLocalRef CommandsArray(Environment,Environment->NewObjectArray(static_cast<jsize>(CommandList.size), Self->m_CommandClass, nullptr));
    
        for (unsigned int i = 0; i < CommandList.size; ++i)
        {
            const auto& [CommandType, GPUState, GeometryID, IndicesCount, IndicesOffset] = CommandList.commands[i];
            const CScopedLocalRef CommandObject(Environment, Environment->NewObject(Self->m_CommandClass, Self->m_CommandConstructorID));
            
            const CScopedLocalRef TypeValue(Environment, Environment->GetObjectArrayElement(TypeValues, CommandType));
            Environment->SetObjectField(CommandObject, TypeFieldID, TypeValue.Get());
            
            const CScopedLocalRef StateObject(Environment, BuildGPUState(Environment, Self, GPUState));
            Environment->SetObjectField(CommandObject, StateFieldID, StateObject.Get());
    
            Environment->SetIntField(CommandObject, GeometryIDFieldID, static_cast<jint>(GeometryID));
            Environment->SetIntField(CommandObject, IndicesCountFieldID, static_cast<jint>(IndicesCount));
            Environment->SetIntField(CommandObject, IndicesOffsetFieldID, static_cast<jint>(IndicesOffset));
    
            Environment->SetObjectArrayElement(CommandsArray, static_cast<jsize>(i), CommandObject.Get());
        }
    
        Environment->CallVoidMethod(Self->m_JavaImplementation, Self->m_UpdateCommandListMethodID, CommandsArray.Get());
        
        CheckException(Environment);
    }
    
    /*------------------------*/
    /* -- Helper Functions -- */
    /*------------------------*/
    
    jbyteArray CGPUDriverCallbackAdapter::BytesToJavaArray(JNIEnv* Environment, const void* Data, size_t Size)
    {
        const jbyteArray Array = Environment->NewByteArray(static_cast<jsize>(Size));
        if (!Array)
            return nullptr;
        
        Environment->SetByteArrayRegion(Array, 0, static_cast<jsize>(Size), static_cast<const jbyte*>(Data));
        
        return Array;
    }

    jobject CGPUDriverCallbackAdapter::BuildGPUState(JNIEnv* Environment, const CGPUDriverCallbackAdapter* Self, const ULGPUState& GPUState)
    {
        const CScopedLocalRef StateObject(Environment, Environment->NewObject(Self->m_GPUStateClass, Self->m_GPUStateConstructorID));
        
        auto IntField = [&](const char* Name, jint Value) {
            Environment->SetIntField(StateObject, Environment->GetFieldID(Self->m_GPUStateClass, Name, "I"), Value);
        };
            
        auto BoolField = [&](const char* Name, jboolean Value) {
            Environment->SetBooleanField(StateObject, Environment->GetFieldID(Self->m_GPUStateClass, Name, "Z"), Value);
        };
            
        auto FloatArrayField = [&](const char* Name, const float* Data, int Length){
            const jfieldID FieldID = Environment->GetFieldID(Self->m_GPUStateClass, Name, "[F");
            const CScopedLocalRef Array(Environment, Environment->NewFloatArray(Length));
            Environment->SetFloatArrayRegion(Array, 0, Length, Data);
            Environment->SetObjectField(StateObject, FieldID, Array.Get());
        };
    
        IntField("viewportWidth", static_cast<jint>(GPUState.viewport_width));
        IntField("viewportHeight", static_cast<jint>(GPUState.viewport_height));
        FloatArrayField("transform", GPUState.transform.data, 16);
        BoolField("enableTexturing", GPUState.enable_texturing);
        BoolField("enableBlend", GPUState.enable_blend);
        
        // Shader Type
        const CScopedLocalRef ShaderTypeObj(Environment, Environment->CallStaticObjectMethod(Self->m_ShaderTypeEnumClass,
                                    Self->m_ShaderTypeFromNativeMethodID, static_cast<jint>(GPUState.shader_type)));
        
        Environment->SetObjectField(StateObject, Self->m_GPUStateShaderTypeFieldID, ShaderTypeObj.Get());
        
        IntField("renderBufferId", static_cast<jint>(GPUState.render_buffer_id));
        IntField("texture1Id", static_cast<jint>(GPUState.texture_1_id));
        IntField("texture2Id", static_cast<jint>(GPUState.texture_2_id));
        IntField("texture3Id", static_cast<jint>(GPUState.texture_3_id));
        FloatArrayField("uniformScalar", GPUState.uniform_scalar, 8);
    
        // uniform_vector: 8 × vec4 → flatten to float[32]
        float UniformVecFlat[32];
        for (int i = 0; i < 8; ++i)
            std::copy_n(GPUState.uniform_vector[i].value, 4, UniformVecFlat + i * 4);
        
        FloatArrayField("uniformVector", UniformVecFlat, 32);
    
        IntField("clipSize", GPUState.clip_size);
    
        // clip: 8 × Matrix4x4 → flatten to float[128]
        float ClipFlat[128];
        for (int i = 0; i < 8; ++i)
            std::copy_n(GPUState.clip[i].data, 16, ClipFlat + i * 16);
        
        FloatArrayField("clip", ClipFlat, 128);
    
        BoolField("enableScissor", GPUState.enable_scissor);
    
        const CScopedLocalRef Rect(Environment, Environment->NewObject(Self->m_IntRectClass, Self->m_IntRectConstructorID,
                           static_cast<jint>(GPUState.scissor_rect.left),  static_cast<jint>(GPUState.scissor_rect.top),
                           static_cast<jint>(GPUState.scissor_rect.right), static_cast<jint>(GPUState.scissor_rect.bottom)));
            
        Environment->SetObjectField(StateObject, Environment->GetFieldID(Self->m_GPUStateClass, "scissorRect", "Lme/ayydxn/luminescence/geometry/IntRect;"),
            Rect.Get());
    
        return StateObject.Get();
    }

    jobject CGPUDriverCallbackAdapter::BuildVertexBuffer(JNIEnv* Environment, const CGPUDriverCallbackAdapter* Self, const ULVertexBuffer& VertexBuffer)
    {
        const CScopedLocalRef FormatObject(Environment, Environment->CallStaticObjectMethod(Self->m_VertexFormatClass, Self->m_VertexFormatFromNativeID,
                                    static_cast<jint>(VertexBuffer.format)));
        
        const CScopedLocalRef DirectBuf(Environment, Environment->NewDirectByteBuffer(VertexBuffer.data, VertexBuffer.size));

        return Environment->NewObject(Self->m_VertexBufferClass, Self->m_VertexBufferConstructorID, FormatObject.Get(), DirectBuf.Get());
    }

    jobject CGPUDriverCallbackAdapter::BuildIndexBuffer(JNIEnv* Environment, const CGPUDriverCallbackAdapter* Self, const ULIndexBuffer& IndexBuffer)
    {
        const CScopedLocalRef DataArr(Environment, BytesToJavaArray(Environment, IndexBuffer.data, IndexBuffer.size));
        
        return Environment->NewObject(Self->m_IndexBufferClass, Self->m_IndexBufferConstructorID, DataArr.Get());
    }
}
