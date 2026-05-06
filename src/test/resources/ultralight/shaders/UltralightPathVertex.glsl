#version 450 core

// ============================================================================
// Ultralight FillPath Vertex Shader
// Converted from vertex_path.hlsl — uniforms via UBO matching GPUDriverGL.
//
// Vertex format: kVertexBufferFormat_2f_4ub_2f  (stride = 20 bytes)
// ============================================================================

layout(location = 0) in vec2  in_pos;
layout(location = 1) in vec4  in_color;   // normalized ubyte [0,1]
layout(location = 2) in vec2  in_obj;

layout(std140, binding = 0) uniform type_Uniforms
{
    vec4  State;
    mat4  Transform;    // Combined ortho × object transform, column-major.
    ivec4 Integer4[2];
    vec4  Scalar4[2];
    vec4  Vector[8];
    ivec4 ClipData;
    mat4  Clip[8];
} Uniforms;

out vec4 v_color;
out vec2 v_tex;
out vec2 v_obj;
out vec4 v_data0;
out vec4 v_data1;
out vec4 v_data2;
out vec4 v_data3;
out vec4 v_data4;
out vec4 v_data5;
out vec4 v_data6;

void main()
{
    gl_Position = Uniforms.Transform * vec4(in_pos, 0.0, 1.0);
    gl_Position.y = -gl_Position.y;  // Invert Y for OpenGL's coordinate system.

    v_color = in_color;
    v_obj   = in_obj;

    // Path shader doesn't use these — zero them for the fragment stage.
    v_tex   = vec2(0.0);
    v_data0 = vec4(0.0);
    v_data1 = vec4(0.0);
    v_data2 = vec4(0.0);
    v_data3 = vec4(0.0);
    v_data4 = vec4(0.0);
    v_data5 = vec4(0.0);
    v_data6 = vec4(0.0);
}
