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
        set_shader_uf_int(3, "uf_txd_unit", t1->texture_info_ptr->unit);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    {
        vec2 pos = { 000.0f, 160.0f };
        set_shader_uf_vec2(3, "uf_model_pos", pos);
        vec2 size = { 150.0f, 150.0f };
        set_shader_uf_vec2(3, "uf_model_size", size);
        set_shader_uf_int(3, "uf_txd_array_z_offset", t2->texture_info_ptr->z_offset);
        set_shader_uf_int(3, "uf_txd_unit", t2->texture_info_ptr->unit);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    {
        vec2 pos = { 000.0f, 320.0f };
        set_shader_uf_vec2(3, "uf_model_pos", pos);
        vec2 size = { 150.0f, 150.0f };
        set_shader_uf_vec2(3, "uf_model_size", size);
        set_shader_uf_int(3, "uf_txd_array_z_offset", t3->texture_info_ptr->z_offset);
        set_shader_uf_int(3, "uf_txd_unit", t3->texture_info_ptr->unit);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    {
        vec2 pos = { 160.0f, 000.0f };
        set_shader_uf_vec2(3, "uf_model_pos", pos);
        vec2 size = { 150.0f, 150.0f };
        set_shader_uf_vec2(3, "uf_model_size", size);
        set_shader_uf_int(3, "uf_txd_array_z_offset", t4->texture_info_ptr->z_offset);
        set_shader_uf_int(3, "uf_txd_unit", t4->texture_info_ptr->unit);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    {
        vec2 pos = { 160.0f, 160.0f };
        set_shader_uf_vec2(3, "uf_model_pos", pos);
        vec2 size = { 150.0f, 150.0f };
        set_shader_uf_vec2(3, "uf_model_size", size);
        set_shader_uf_int(3, "uf_txd_array_z_offset", t5->texture_info_ptr->z_offset);
        set_shader_uf_int(3, "uf_txd_unit", t5->texture_info_ptr->unit);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    {
        vec2 pos = { 160.0f, 320.0f };
        set_shader_uf_vec2(3, "uf_model_pos", pos);
        vec2 size = { 150.0f, 150.0f };
        set_shader_uf_vec2(3, "uf_model_size", size);
        set_shader_uf_int(3, "uf_txd_array_z_offset", t6->texture_info_ptr->z_offset);
        set_shader_uf_int(3, "uf_txd_unit", t6->texture_info_ptr->unit);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    {
        vec2 pos = { 320.0f, 000.0f };
        set_shader_uf_vec2(3, "uf_model_pos", pos);
        vec2 size = { 150.0f, 150.0f };
        set_shader_uf_vec2(3, "uf_model_size", size);
        set_shader_uf_int(3, "uf_txd_array_z_offset", t7->texture_info_ptr->z_offset);
        set_shader_uf_int(3, "uf_txd_unit", t7->texture_info_ptr->unit);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    {
        vec2 pos = { 320.0f, 160.0f };
        set_shader_uf_vec2(3, "uf_model_pos", pos);
        vec2 size = { 150.0f, 150.0f };
        set_shader_uf_vec2(3, "uf_model_size", size);
        set_shader_uf_int(3, "uf_txd_array_z_offset", t8->texture_info_ptr->z_offset);
        set_shader_uf_int(3, "uf_txd_unit", t8->texture_info_ptr->unit);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    {
        vec2 pos = { 320.0f, 320.0f };
        set_shader_uf_vec2(3, "uf_model_pos", pos);
        vec2 size = { 150.0f, 150.0f };
        set_shader_uf_vec2(3, "uf_model_size", size);
        set_shader_uf_int(3, "uf_txd_array_z_offset", t9->texture_info_ptr->z_offset);
        set_shader_uf_int(3, "uf_txd_unit", t9->texture_info_ptr->unit);
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

    /*map_item map[] = {
        {"str1", 1}, {"str2", 2}, {"str3", 3}, {"str4", 4},
        {"str5", 5}, {"str6", 6}, {"str7", 7}, {"str8", 8}
    };*/


    unsigned int shader_program = create_shader_program(
        "resources/shaders/txd_array_vertex.shader",
        "resources/shaders/txd_array_fragment.shader");

    use_shader_program(shader_program);

    // Uncomment to check for memory leaks.
    //while (1)
    //{
    //    t1 = tb_add_texture(TB_NO_GROUP, "resources/img/512x512_transp.png", 0, 0, 512, 512);
    //    t2 = tb_add_texture(TB_NO_GROUP, "resources/img/256x256.jpg", 0, 0, 256, 256);
    //    t3 = tb_add_texture(TB_NO_GROUP, "resources/img/256x256.jpg", 128, 128, 128, 128);
    //    /* The next 3 textures are guaranteed to be placed on the same layer */
    //    t4 = tb_add_texture(1, "resources/img/512x512_transp.png", 0, 0, 256, 256);
    //    t5 = tb_add_texture(1, "resources/img/512x512_transp.png", 128, 0, 256, 256);
    //    t6 = tb_add_texture(1, "resources/img/512x512_transp.png", 256, 0, 256, 256);
    //    /* The next 3 textures are guaranteed to be placed on the same layer */
    //    t7 = tb_add_texture(2, "resources/img/512x512_transp.png", 0, 256, 256, 256);
    //    t8 = tb_add_texture(2, "resources/img/512x512_transp.png", 128, 256, 256, 256);
    //    t9 = tb_add_texture(2, "resources/img/512x512_transp.png", 256, 256, 256, 256);
    //    tb_build();
    //    //tb_destroy();
    //    int bp = 0;
    //}


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
    for (int i = 0; i < 100; i++)
    {
        tb_add_texture(0, "resources/img/512x512_transp.png",
            rand() % 449, rand() % 449, 1 + rand() % 62, 1 + rand() % 62);
    }
    tb_build();

    /* Set up vertices and txd vertices data */
    float vertices[] =
    {
        1.0f, 0.0f,                     /* Top right                          */
        1.0f, 1.0f,                     /* Bottom right                       */
        0.0f, 1.0f,                     /* Bottom left                        */
        0.0f, 0.0f                      /* Top left                           */
    };

    float txd_vertices[] =
    {
        1.0f, 1.0f,                     /* Top right                          */
        1.0f, 0.0f,                     /* Bottom right                       */
        0.0f, 0.0f,                     /* Bottom left                        */
        0.0f, 1.0f                      /* Top left                           */
    };

    unsigned int va = va_create();
    va_add_textured_rect(va, vertices, txd_vertices);
    va_build(va);

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
