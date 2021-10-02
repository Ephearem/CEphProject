#version 430 core

out vec4 fs_out_color;

in vec2 vs_out_txd_pos;

uniform sampler2DArray uf_txd_unit;
uniform int uf_txd_array_z_offset;

void main()
{
    fs_out_color = texture(uf_txd_unit, vec3(vs_out_txd_pos, uf_txd_array_z_offset));
}
