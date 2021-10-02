#ifndef SHADERS_H
#define SHADERS_H



unsigned int create_shader_program(const char* vertex_shader_path,
    const char* fragment_shader_path);
void use_shader_program(unsigned int shader_program);



#endif /* !SHADERS_H */
