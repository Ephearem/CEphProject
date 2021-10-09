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
#include "core/graphics/image.h"



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
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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


    unsigned int texture_2d_array;
    glGenTextures(1, &texture_2d_array);
                                        /* Generate a 2d texture array        */
                                        /* object                             */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_2d_array);
                                        /* Set 'texture_2d_array' as the      */
                                        /* current vertex array object.       */
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage3D(
        GL_TEXTURE_2D_ARRAY,            /* target to which the texture is     */
                                        /* bound                              */
        0,                              /* level                              */
        GL_RGBA8,                       /* Internal format                    */
        (GLsizei)512,                   /* Width of the 2d texture array      */
        (GLsizei)512,                   /* Heigh of the 2d texture array      */
        (GLsizei)2,                     /* Depth of the 2d texture array      */
        0,                              /* Border, must be 0.                 */
        GL_RGBA,                        /* Format of the pixel data           */
        GL_UNSIGNED_BYTE,               /* Data type of the pixel data        */
        NULL);                          /* A pointer to the image data        */


    const stImage* img_1 = load_image("resources/img/512x512_transp.png");
    const stImage* img_2 = load_image("resources/img/256x256.jpg");

                                        /* Add texture to the 0th layer of    */
                                        /* the 2d texture array               */

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 512);
                                        /* The full width of the image from   */
                                        /* which the texture is created       */
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
                                        /* Subtexture x-offset (from the      */
                                        /* beginning of the image).           */
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
                                        /* Subtexture y-offset (from the      */
                                        /* beginning of the image).           */
    glTexSubImage3D(
        GL_TEXTURE_2D_ARRAY,            /* target to which the texture is     */
                                        /* bound                              */
        0,                              /* Level-of-detail. 0 - base image    */
        0,                              /* x-offset within the texture array  */
        0,                              /* y offset within the texture array  */
        0,                              /* z offset (layer)                   */
        (GLsizei)256,                   /* width of the texture subimage      */
        (GLsizei)512,                   /* height of the texture subimage     */
        1,                              /* depth of the texture subimage      */
        GL_RGBA,                        /* Format of the pixel data           */
        GL_UNSIGNED_BYTE,               /* Data type of the pixel data        */
        (const void*)img_1->data_ptr);  /* A pointer to the image data        */


                                        /* Add texture to the 1st layer of    */
                                        /* the 2d texture array               */

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_2d_array);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 256);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glTexSubImage3D(
        GL_TEXTURE_2D_ARRAY,
        0,
        256,
        256,                            /* The offset is calculated from the  */
                                        /* bottom of the texture. For it to   */
                                        /* be considered from the top, need   */
                                        /* to swap the vertices related to    */
                                        /* the texture in the 'vertices'      */
                                        /* array.                             */
        // TODO: Check out the comment posted above because I don't remember
        //       exactly :)
        0,
        (GLsizei)256,
        (GLsizei)256,
        1,
        GL_RGB,                         /* There are only 3 channels in this  */
                                        /* .jpg image                         */
        GL_UNSIGNED_BYTE,
        (const void*)img_2->data_ptr);

    free_image(img_1);
    free_image(img_2);

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
    glDeleteTextures(1, &texture_2d_array); // TODO: Implement.

    return 0;
}
#endif /* !TEST_RUN */
