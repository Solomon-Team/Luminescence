#version 450 core

// ============================================================================
// Ultralight Fill Vertex Shader
// Converted from vertex_quad.hlsl — uniforms via UBO matching GPUDriverGL.
//
// Vertex format: kVertexBufferFormat_2f_4ub_2f_2f_28f  (stride = 140 bytes)
//
//  location  attrib   type              offset  bytes
//  --------  ------   ----              ------  -----
//     0      pos      vec2 float           0      8
//     1      color    vec4 ubyte norm      8      4
//     2      tex      vec2 float          12      8
//     3      obj      vec2 float          20      8
//     4      data0    vec4 float          28     16
//   5-10   data1-6    vec4 float        44-124   16×6
//                                               ---
//                                               140
// ============================================================================

layout(location = 0)  in vec2  in_pos;
layout(location = 1)  in vec4  in_color;   // normalized ubyte [0,1]
layout(location = 2)  in vec2  in_tex;
layout(location = 3)  in vec2  in_obj;
layout(location = 4)  in vec4  in_data0;
layout(location = 5)  in vec4  in_data1;
layout(location = 6)  in vec4  in_data2;
layout(location = 7)  in vec4  in_data3;
layout(location = 8)  in vec4  in_data4;
layout(location = 9)  in vec4  in_data5;
layout(location = 10) in vec4  in_data6;

// Uniform block — matches struct Uniforms layout in GPUDriverGL.h (std140).
// Bound to binding point 0 via glUniformBlockBinding.
layout(std140, binding = 0) uniform type_Uniforms
{
    vec4  State;        // (time, screenWidth, screenHeight, screenScale)
    mat4  Transform;    // Combined ortho-projection × object-space transform.
                        // Uploaded column-major (matches GLSL mat4 memory order).
    ivec4 Integer4[2];
    vec4  Scalar4[2];
    vec4  Vector[8];
    ivec4 ClipData;     // x = clip count
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
    /*
     * Transform is the result of ApplyProjection() from the Java driver:
     * an orthographic matrix (pixel-space → NDC, Y-flipped) multiplied by
     * the per-draw object-space affine matrix from ULGPUState.transform.
     * It is stored column-major, matching GLSL's native mat4 layout,
     * so no transpose is needed.
     */
    gl_Position = Uniforms.Transform * vec4(in_pos, 0.0, 1.0);
    gl_Position.y = -gl_Position.y;  // Invert Y for OpenGL's coordinate system.

    v_color = in_color;
    v_tex   = in_tex;
    v_obj   = in_obj;
    v_data0 = in_data0;
    v_data1 = in_data1;
    v_data2 = in_data2;
    v_data3 = in_data3;
    v_data4 = in_data4;
    v_data5 = in_data5;
    v_data6 = in_data6;
}
