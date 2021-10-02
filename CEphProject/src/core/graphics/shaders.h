#ifndef SHADERS_H
#define SHADERS_H

#pragma warning (disable: 4996)
#include <cglm/cglm.h>


unsigned int create_shader_program(const char* vertex_shader_path,
    const char* fragment_shader_path);
void use_shader_program(unsigned int shader_program);

void set_shader_uf_int(
    unsigned int shader_program, const char* name, int value);
void set_shader_uf_float(
    unsigned int shader_program, const char* name, float value);
void set_shader_uf_vec2(
    unsigned int shader_program, const char* name, vec2 value);
void set_shader_uf_vec3(
    unsigned int shader_program, const char* name, vec3 value);
void set_shader_uf_mat4(
    unsigned int shader_program, const char* name, mat4 value);



#endif /* !SHADERS_H */
