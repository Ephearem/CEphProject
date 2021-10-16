#ifndef SHADER_H
#define SHADER_H

#pragma warning (disable: 4996)
#include <cglm/cglm.h>


unsigned int shader_create_program(const char* vertex_shader_path,
    const char* fragment_shader_path);
void shader_use_program(unsigned int shader_program);

void shader_set_uf_int(
    unsigned int shader_program, const char* name, int value);
void shader_set_uf_float(
    unsigned int shader_program, const char* name, float value);
void shader_set_uf_fvec2(
    unsigned int shader_program, const char* name, vec2 value);
void shader_set_uf_fvec3(
    unsigned int shader_program, const char* name, vec3 value);
void shader_set_uf_fmat4(
    unsigned int shader_program, const char* name, mat4 value);



#endif /* !SHADER_H */
