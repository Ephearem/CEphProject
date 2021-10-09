/**-----------------------------------------------------------------------------
; @file main.c
;
; @brief
;   Contains the entry point to the program.
;
; @date   September 2021
; @author Eph
;
-----------------------------------------------------------------------------**/

#include <glad\glad.h>  // TODO: Remove.
#include <GLFW/glfw3.h> // TODO: Remove.

#pragma warning (disable: 4996)
#include <cglm/cglm.h>

#include "core/window.h"
#include "core/loop.h"
#include "core/graphics/shaders.h"
#include "core/graphics/texture/texture_builder.h"



stTexture* t1 = NULL;
stTexture* t2 = NULL;
stTexture* t3 = NULL;



/** @functions  ------------------------------------------------------------**/

/**-----------------------------------------------------------------------------
; @func loop_iteration_callback
;
; @brief
;   This function is called at every tick of the main loop.
;
-----------------------------------------------------------------------------**/
void loop_iteration_callback(void)
{
    {
        vec2 pos = { 000.0f, 000.0f };
        set_shader_uf_vec2(3, "uf_model_pos", pos);
        vec2 size = { 150.0f, 150.0f };
        set_shader_uf_vec2(3, "uf_model_size", size);
        set_shader_uf_int(3, "uf_txd_array_z_offset", t1->texture_info_ptr->z_offset);
        set_shader_uf_int(3, "uf_txd_unit", t1->texture_info_ptr->unit - GL_TEXTURE0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    {
        vec2 pos = { 200.0f, 200.0f };
        set_shader_uf_vec2(3, "uf_model_pos", pos);
        vec2 size = { 150.0f, 150.0f };
        set_shader_uf_vec2(3, "uf_model_size", size);
        set_shader_uf_int(3, "uf_txd_array_z_offset", t2->texture_info_ptr->z_offset);
        set_shader_uf_int(3, "uf_txd_unit", t2->texture_info_ptr->unit - GL_TEXTURE0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    {
        vec2 pos = { 400.0f, 400.0f };
        set_shader_uf_vec2(3, "uf_model_pos", pos);
        vec2 size = { 150.0f, 150.0f };
        set_shader_uf_vec2(3, "uf_model_size", size);
        set_shader_uf_int(3, "uf_txd_array_z_offset", t3->texture_info_ptr->z_offset);
        set_shader_uf_int(3, "uf_txd_unit", t3->texture_info_ptr->unit - GL_TEXTURE0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

}


/**-----------------------------------------------------------------------------
; @func main
;
; @brief
;   Entry point.
;
-----------------------------------------------------------------------------**/
#ifndef TEST_RUN
int main(int argc, char* argv[], char* envp[])
{
    init_window("CEphProject", 800, 600, 0, 0);


    unsigned int shader_program = create_shader_program(
        "resources/shaders/txd_array_vertex.shader",
        "resources/shaders/txd_array_fragment.shader");

    use_shader_program(shader_program);

    t1 = tb_add_texture("resources/img/512x512_transp.png", 0, 0, 512, 512);
    t2 = tb_add_texture("resources/img/256x256.jpg", 0, 0, 256, 256);
    t3 = tb_add_texture("resources/img/512x512_transp.png", 128, 128, 384, 384);

    for (int i = 0; i < 100; i++)
    {
        tb_add_texture("resources/img/512x512_transp.png",
            rand() % 449, rand() % 449, 1 + rand() % 62, 1 + rand() % 62);
    }
    tb_build();
    
    /* Set up vertices and indices data */
    float vertices[] =
    {
        1.0f, 0.0f,                     /* Top right                          */
        1.0f, 1.0f,                     /* Bottom right                       */
        0.0f, 1.0f,                     /* Bottom left                        */
        0.0f, 0.0f                      /* Top left                           */
    };
    unsigned int indices[] =
    {
        0, 1, 3,                        /* The 1-st triangle                  */
        1, 2, 3                         /* The 2-nd triangle                  */
    };
    float txd_vertices[] =
    {
        1.0f, 1.0f,                     /* Top right                          */
        1.0f, 0.0f,                     /* Bottom right                       */
        0.0f, 0.0f,                     /* Bottom left                        */
        0.0f, 1.0f                      /* Top left                           */
    };

    unsigned int vertex_buffer = 0;
    unsigned int txd_vertex_buffer = 0;
    unsigned int indices_buffer = 0;
    unsigned int vertex_array = 0;

    glGenVertexArrays(1, &vertex_array);/* Generate a verex array object      */

    glGenBuffers(1, &vertex_buffer);    /* Generate a buffer object to store  */
                                        /* the positions of the vertices      */

    glGenBuffers(1, &txd_vertex_buffer);/* Generate a buffer object to store  */
                                        /* the texture coordinates            */

    glGenBuffers(1, &indices_buffer);   /* Generate a buffer object to store  */
                                        /* the vertex indices                 */

    glBindVertexArray(vertex_array);    /* Set 'vertex_array' as the current  */
                                        /* vertex array object.               */

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
                                        /* Bind 'vertex_buffer' to            */
                                        /* GL_ARRAY_BUFFER. All following     */
                                        /* calls to GL_ARRAY_BUFFER will      */
                                        /* refer to this 'vertex_buffer'      */
                                        /* object.                            */

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                                        /* Put data from 'vertices' into      */
                                        /* GL_ARRAY_BUFFER (i.e. into         */
                                        /* 'vertex_buffer')                   */

    glBindBuffer(GL_ARRAY_BUFFER, txd_vertex_buffer);
                                        /* Bind 'vertex_buffer' to            */
                                        /* GL_ARRAY_BUFFER. All following     */
                                        /* calls to GL_ARRAY_BUFFER will      */
                                        /* refer to this 'vertex_buffer'      */
                                        /* object.                            */
    glBufferData(GL_ARRAY_BUFFER, sizeof(txd_vertices), txd_vertices,
        GL_STATIC_DRAW);
                                        /* Put data from 'txd_vertices' into  */
                                        /* GL_ARRAY_BUFFER (i.e. into         */
                                        /* 'txd_vertex_buffer'                */

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
                                        /* Bind 'vertex_buffer' to            */
                                        /* GL_ELEMENT_ARRAY_BUFFER. All       */
                                        /* following calls to                 */
                                        /* GL_ELEMENT_ARRAY_BUFFER will refer */
                                        /* to this 'indices_buffer' object.   */

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
        GL_STATIC_DRAW);                /* Put data from 'indices' into       */
                                        /* GL_ELEMENT_ARRAY_BUFFER (i.e. into */
                                        /* 'indices_buffer')                  */

    glBindVertexBuffer(0, vertex_buffer, 0, sizeof(GLfloat) * 2);
                                        /* Bind 'vertex_buffer' to            */
                                        /* 'vertex_array' at index 0.         */

    glBindVertexBuffer(1, txd_vertex_buffer, 0, sizeof(GLfloat) * 2);
                                        /* Bind 'txd_vertex_buffer' to        */
                                        /* 'vertex_array' at index 0.         */

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);   /* 'vertex_buffer' and                */
                                        /* 'txd_vertex_buffer'can be unbound  */
                                        /* since they are bound to            */
                                        /* 'vertex_array' as the vertex       */
                                        /* attributes at indices 0 and 1      */

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                                        /* 'indices_buffer' cannot be         */
                                        /* unbound from 'vertex_array' while  */
                                        /* 'vertex_array' is active           */


    glBindVertexArray(0);               /* Unbind 'vertex_array' so that      */
                                        /* other calls to the vertex array    */
                                        /* will not change the current        */
                                        /* ('vertex_array') object.           */
    glBindVertexArray(vertex_array);


    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_ortho(0.0f, (float)get_window_width(), (float)get_window_height(),
        0.0f, -0.1f, 0.1f, projection);
    set_shader_uf_mat4(shader_program, "uf_projection", projection);
    set_shader_uf_int(shader_program, "uf_txd_array_z_offset", 0);
    set_shader_uf_int(shader_program, "uf_txd_unit", 0);
    vec2 pos = { 10.0f, 10.0f };
    set_shader_uf_vec2(shader_program, "uf_model_pos", pos);
    vec2 size = { 512.0f, 512.0f };
    set_shader_uf_vec2(shader_program, "uf_model_size", size);


    start_loop(loop_iteration_callback);

    /* De-allocate all resources */
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &txd_vertex_buffer);
    glDeleteBuffers(1, &indices_buffer);
    // glDeleteTextures(1, &texture_2d_array); // TODO: Implement.

    return 0;
}
#endif /* !TEST_RUN */
