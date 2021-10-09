#version 430 core

layout(location = 0) in vec2 in_pos;    /* Local space                        */
layout(location = 1) in vec2 in_txd_pos;

uniform mat4 uf_projection;
uniform vec2 uf_model_pos;
uniform vec2 uf_model_size;
// TODO: Use vec4 to pass position and size to the shader to reduce the number
//       of calls to the GPU.

out vec2 vs_out_txd_pos;

void main()
{
    mat4 model;
    /* Scaling                           */
    model[0][0] = uf_model_size.x;      /* x                                  */
    model[1][1] = uf_model_size.y;      /* y                                  */
    model[2][2] = 1.0f;
    model[3][3] = 1.0f;

    /* Translation                       */
    model[3][0] = uf_model_pos.x;       /* x                                  */
    model[3][1] = uf_model_pos.y;       /* y                                  */

    gl_Position = uf_projection * model * vec4(in_pos, 0.0, 1.0);
    vs_out_txd_pos = in_txd_pos;
}
