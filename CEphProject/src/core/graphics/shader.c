// TODO: Place the result of 'glGetUniformLocation' for each uniform in map.
//       On subsequent calls to this variable, take the value from map. This
//       will reduce the number of calls to the GPU.



#include <stdio.h>
#include <stdlib.h>

#include <glad\glad.h>

#include "shader.h"
#include "../../log.h"



/** @internal_prototypes -----------------------------------------------------*/
static unsigned int _compile_shader(const char* shader_source,
    GLenum shader_type);
static unsigned int _link_shader_program(unsigned int vertex_shader,
    unsigned int fragment_shader);



/** @functions ---------------------------------------------------------------*/
unsigned int shader_create_program(const char* vertex_shader_path,
    const char* fragment_shader_path)
{
    FILE* vertex_shader_file = NULL;
    fopen_s(&vertex_shader_file, vertex_shader_path, "rb");
    char* vertex_shader_source = NULL;
    if (vertex_shader_file)
    {
        fseek(vertex_shader_file, 0, SEEK_END);
        long fsize = ftell(vertex_shader_file);
        fseek(vertex_shader_file, 0, SEEK_SET);  /* same as rewind(f); */

        vertex_shader_source = malloc(fsize + 1);
        fread(vertex_shader_source, 1, fsize, vertex_shader_file);
        fclose(vertex_shader_file);
        vertex_shader_source[fsize] = 0;
    }
    else
    {
        LOG_ERROR("Unable to open vertex shader file: %s", vertex_shader_path);
        return 0;
    }
    FILE* fragment_shader_file = NULL;
    fopen_s(&fragment_shader_file, fragment_shader_path, "rb");
    char* fragment_shader_source = NULL;
    if (fragment_shader_file)
    {
        fseek(fragment_shader_file, 0, SEEK_END);
        long fsize = ftell(fragment_shader_file);
        fseek(fragment_shader_file, 0, SEEK_SET);  /* same as rewind(f); */

        fragment_shader_source = malloc(fsize + 1);
        fread(fragment_shader_source, 1, fsize, fragment_shader_file);
        fclose(fragment_shader_file);
        fragment_shader_source[fsize] = 0;
    }
    else
    {
        LOG_ERROR("Unable to open fragment shader file: %s",
            fragment_shader_path);
        return 0;
    }

    unsigned int vertex_shader = _compile_shader(vertex_shader_source,
        GL_VERTEX_SHADER);
    unsigned int fragment_shader = _compile_shader(fragment_shader_source,
        GL_FRAGMENT_SHADER);
    unsigned int shader_program = _link_shader_program(vertex_shader, 
       fragment_shader);
   
    GL_CALL(glDeleteShader(vertex_shader));
    GL_CALL(glDeleteShader(fragment_shader));

    free(vertex_shader_source);
    free(fragment_shader_source);

    return shader_program;
}


void shader_use_program(unsigned int shader_program)
{
    glUseProgram(shader_program); // TODO: Use 'GL_CALL' macro.
}


void shader_set_uf_int(unsigned int shader_program, const char* name, int value)
{
    int uniform_location = glGetUniformLocation(shader_program, name);
    glUniform1i(uniform_location, value);
}


void shader_set_uf_float(unsigned int shader_program, const char* name, float value)
{
    int uniform_location = glGetUniformLocation(shader_program, name);
    glUniform1f(uniform_location, value);
}


void shader_set_uf_fvec2(unsigned int shader_program, const char* name, vec2 value)
{
    int uniform_location = glGetUniformLocation(shader_program, name);
    glUniform2fv(uniform_location, 1, value);
}


void shader_set_uf_fvec3(unsigned int shader_program, const char* name, vec3 value)
{
    int uniform_location = glGetUniformLocation(shader_program, name);
    glUniform3fv(uniform_location, 1, value);
}


void shader_set_uf_fmat4(unsigned int shader_program, const char* name, mat4 value)
{
    int uniform_location = glGetUniformLocation(shader_program, name);
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, value);
}



static unsigned int _compile_shader(const char* shader_source,
    GLenum shader_type)
{
    int result = 0;
    char info_log[512]; // TODO: 512 hardcode.

    unsigned int shader = glCreateShader(shader_type);
    GL_CALL(glShaderSource(shader, 1, &shader_source, NULL));
    GL_CALL(glCompileShader(shader));

    GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
    if (0 == result)
    {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        LOG_ERROR("Shader compilation failed.\n%s", info_log);
        return 0;
    }
    return shader;
}


static unsigned int _link_shader_program(unsigned int vertex_shader,
    unsigned int fragment_shader)
{
    int result = 0;
    char info_log[512]; // TODO: 512 hardcode.

    unsigned int shader_program = glCreateProgram();
    GL_CALL(glAttachShader(shader_program, vertex_shader));
    GL_CALL(glAttachShader(shader_program, fragment_shader));
    GL_CALL(glLinkProgram(shader_program));

    GL_CALL(glGetProgramiv(shader_program, GL_LINK_STATUS, &result));
    if (0 == result)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        LOG_ERROR("Shader program link failed.\n%s", info_log);
        return 0;
    }
    return shader_program;
}
