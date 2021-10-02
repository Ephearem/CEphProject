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
        "resources/shaders/default_vertex.shader",
        "resources/shaders/default_fragment.shader");

    use_shader_program(shader_program);


    /* Set up vertices and indices data */
    float vertices[] =                  
    {
         0.5f,  0.5f,                   /* Top right                          */
         0.5f, -0.5f,                   /* Bottom right                       */
        -0.5f, -0.5f,                   /* Bottom left                        */
        -0.5f,  0.5f,                   /* Top left                           */
    };
    unsigned int indices[] =
    {
        0, 1, 3,                        /* The 1-st triangle                  */
        1, 2, 3                         /* The 2-nd triangle                  */
    };
    unsigned int vertex_buffer = 0;
    unsigned int indices_buffer = 0;
    unsigned int vertex_array = 0;

    glGenVertexArrays(1, &vertex_array);/* Generate a verex array object      */

    glGenBuffers(1, &vertex_buffer);    /* Generate a buffer object to store  */
                                        /* the positions of the vertices      */

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
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);   /* 'vertex_buffer' can be safely      */
                                        /* unbind since it is bound to        */
                                        /* 'vertex_array' as the vertex       */
                                        /* attribute at index 0.              */

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                                        /* 'indices_buffer' cannot be         */
                                        /* unbound from 'vertex_array' while  */
                                        /* 'vertex_array' is active           */


    glBindVertexArray(0);               /* Unbind 'vertex_array' so that      */
                                        /* other calls to the vertex array    */
                                        /* will not change the current        */
                                        /* ('vertex_array') object.           */
    glBindVertexArray(vertex_array);


    const stImage* img_ptr = load_image("resources/img/512x512_transp.png");
    free_image(img_ptr);


    start_loop(loop_iteration_callback);

    /* De-allocate all resources */
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &indices_buffer);

    return 0;
}
#endif /* !TEST_RUN */
