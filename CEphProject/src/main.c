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
#include "core/graphics/shader.h"
#include "core/graphics/texture/texture_builder.h"
#include "core/graphics/vertex_array.h"



stTexture* t1 = NULL;
stTexture* t2 = NULL;
stTexture* t3 = NULL;
stTexture* t4 = NULL;
stTexture* t5 = NULL;
stTexture* t6 = NULL;
stTexture* t7 = NULL;
stTexture* t8 = NULL;
stTexture* t9 = NULL;

stIndicesInfo* ii1 = NULL;
stIndicesInfo* ii2 = NULL;
stIndicesInfo* ii3 = NULL;


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
        shader_set_uf_fvec2(3, "uf_model_pos", pos);
        vec2 size = { 150.0f, 150.0f };
        shader_set_uf_fvec2(3, "uf_model_size", size);
        shader_set_uf_int(3, "uf_txd_array_z_offset", t1->texture_info_ptr->z_offset);
        shader_set_uf_int(3, "uf_txd_unit", t1->texture_info_ptr->unit);
        glDrawElements(ii1->mode, ii1->count, GL_UNSIGNED_INT, ii1->offset);
    }
    {
        vec2 pos = { 000.0f, 160.0f };
        shader_set_uf_fvec2(3, "uf_model_pos", pos);
        vec2 size = { 150.0f, 150.0f };
        shader_set_uf_fvec2(3, "uf_model_size", size);
        shader_set_uf_int(3, "uf_txd_array_z_offset", t2->texture_info_ptr->z_offset);
        shader_set_uf_int(3, "uf_txd_unit", t2->texture_info_ptr->unit);
        glDrawElements(ii2->mode, ii2->count, GL_UNSIGNED_INT, ii2->offset);
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
    window_init("CEphProject", 800, 600, 0, 0);

    unsigned int shader_program = shader_create_program(
        "resources/shaders/txd_array_vertex.shader",
        "resources/shaders/txd_array_fragment.shader");

    shader_use_program(shader_program);

    /* The next 3 textures will be placed on any free space of any layer of any
       2d texture array */
    t1 = tb_add_texture(TB_NO_GROUP, "resources/img/512x512_transp.png", 0, 0, 512, 512);
    t2 = tb_add_texture(TB_NO_GROUP, "resources/img/256x256.jpg", 0, 0, 256, 256);
    t3 = tb_add_texture(TB_NO_GROUP, "resources/img/256x256.jpg", 128, 128, 128, 128);
    /* The next 3 textures are guaranteed to be placed on the same layer */
    t4 = tb_add_texture(1, "resources/img/512x512_transp.png", 0, 0, 256, 256);
    t5 = tb_add_texture(1, "resources/img/512x512_transp.png", 128, 0, 256, 256);
    t6 = tb_add_texture(1, "resources/img/512x512_transp.png", 256, 0, 256, 256);
    /* The next 3 textures are guaranteed to be placed on the same layer */
    t7 = tb_add_texture(2, "resources/img/512x512_transp.png", 0, 256, 256, 256);
    t8 = tb_add_texture(2, "resources/img/512x512_transp.png", 128, 256, 256, 256);
    t9 = tb_add_texture(2, "resources/img/512x512_transp.png", 256, 256, 256, 256);
    /* The next 100 textures will be placed on any free space of any layer of
       any 2d texture array */
    //for (int i = 0; i < 100; i++)
    //{
    //    tb_add_texture(0, "resources/img/512x512_transp.png",
    //        rand() % 449, rand() % 449, 1 + rand() % 62, 1 + rand() % 62);
    //}
    tb_build();

    /* Set up vertices and txd vertices data */
    float vertices[] =
    {
        1.0f, 0.0f,                     /* Top right                          */
        1.0f, 1.0f,                     /* Bottom right                       */
        0.0f, 1.0f,                     /* Bottom left                        */
        0.0f, 0.0f                      /* Top left                           */
    };
    float vertices2[] =
    {
        0.5f, 0.0f,                     /* Top right                          */
        0.5f, 0.5f,                     /* Bottom right                       */
        0.0f, 0.5f,                     /* Bottom left                        */
        0.0f, 0.0f                      /* Top left                           */
    };
    float vertices3[] =
    {
        1.0f, 0.5f,                     /* Top right                          */
        1.0f, 1.0f,                     /* Bottom right                       */
        0.5f, 1.0f,                     /* Bottom left                        */
        0.5f, 0.5f                      /* Top left                           */
    };
    float txd_vertices[] =
    {
        1.0f, 1.0f,                     /* Top right                          */
        1.0f, 0.0f,                     /* Bottom right                       */
        0.0f, 0.0f,                     /* Bottom left                        */
        0.0f, 1.0f                      /* Top left                           */
    };

    unsigned int va = va_create();
    ii1 = va_shape_create(va);
    ii2 = va_shape_create(va);
    va_shape_add_textured_rect(va, ii1, vertices, txd_vertices);

    va_shape_add_textured_rect(va, ii2, vertices2, t1->vertices);
    va_shape_add_textured_rect(va, ii2, vertices3, t3->vertices);
    va_build(va);

    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_ortho(0.0f, (float)window_get_width(), (float)window_get_height(),
        0.0f, -0.1f, 0.1f, projection);
    shader_set_uf_fmat4(shader_program, "uf_projection", projection);
    shader_set_uf_int(shader_program, "uf_txd_array_z_offset", 0);
    shader_set_uf_int(shader_program, "uf_txd_unit", 0);
    vec2 pos = { 10.0f, 10.0f };
    shader_set_uf_fvec2(shader_program, "uf_model_pos", pos);
    vec2 size = { 512.0f, 512.0f };
    shader_set_uf_fvec2(shader_program, "uf_model_size", size);


    start_loop(loop_iteration_callback);

    /* De-allocate all resources */
    va_destroy(va);
    tb_destroy();
    //glDeleteVertexArrays(1, &vertex_array);
    //glDeleteBuffers(1, &vertex_buffer);
    //glDeleteBuffers(1, &txd_vertex_buffer);
    //glDeleteBuffers(1, &indices_buffer);
    // glDeleteTextures(1, &texture_2d_array); // TODO: Implement.

    return 0;
}
#endif /* !TEST_RUN */
