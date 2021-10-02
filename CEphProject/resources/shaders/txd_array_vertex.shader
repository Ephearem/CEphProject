#version 430 core

layout(location = 0) in vec2 in_pos;    /* Local space                       */
layout(location = 1) in vec2 in_txd_pos;

out vec2 vs_out_txd_pos;

void main()
{
    gl_Position = vec4(in_pos, 0.0, 1.0);
    vs_out_txd_pos = in_txd_pos;
}
