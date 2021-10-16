/**-----------------------------------------------------------------------------
; @file vertex_array.c
;
; @brief
;   The file implements the functionality of the 'vertex_array' module.
;
;   va- vertex array
;   vabd - vertex array build data
;
; @date   October 2021
; @author Eph
;
-----------------------------------------------------------------------------**/

#include <glad/glad.h>

#include "vertex_array.h"
#include "../../containers/map.h"
#include "../memory.h"
#include "../../log.h"



/** @types -------------------------------------------------------------------*/

typedef struct stVaBuildData
{
    unsigned int vertex_array;
    unsigned int vertex_buffer;
    unsigned int txd_vertex_buffer;
    unsigned int indices_buffer;

    size_t vertices_number;
    size_t txd_vertices_number;
    size_t indices_number;

    float* vertices;
    float* txd_vertices;
    unsigned int* indices;

    int indices_offset;

}stVaBuildData;



/** @static_data -------------------------------------------------------------*/

/* Stores information about all vertex arrays to be created */
static map* _va_to_build = NULL;        /* Map of stVaBuildData               */



/** @functions ---------------------------------------------------------------*/

unsigned int va_create(void)
{
    extern map* _va_to_build;

    if (NULL == _va_to_build)
        _va_to_build = map_create();

    stVaBuildData* vabd_ptr = m_calloc(1, sizeof(stVaBuildData));
    
    /* Generate a verex array object  */
    glGenVertexArrays(1, &vabd_ptr->vertex_array);
    if (glGetError() != 0)
    {
        LOG_ERROR("Failed to create vertex array(s).");
        return 0;
    }
    map_insert(_va_to_build, vabd_ptr->vertex_array, vabd_ptr);

    return vabd_ptr->vertex_array;
}



/**-----------------------------------------------------------------------------
; @func va_add_textured_rect
;
; @brief
;   Loads vertices and texture vertices from the arguments to a vertex array.
;   Interprets them as the vertices of quadrangles made up of two triangles.
;   Based on them, this function generates the appropriate indices that can be
;   used to draw a textured quad using the 'glDrawElements' function.
;   Returns the offset to the first index in the generated indices array of this
;   vertex array.
;
-----------------------------------------------------------------------------**/
size_t va_add_textured_rect(unsigned int va_idx, float* vertices, float* txd_vertices)
{
    if (NULL == _va_to_build)
        return;
    stVaBuildData* vabd_ptr = map_search(_va_to_build, va_idx);
    if (NULL == vabd_ptr)
        return;

    vabd_ptr->vertices_number += 8;
    vabd_ptr->txd_vertices_number += 8;
    vabd_ptr->indices_number += 6;


    if (NULL == vabd_ptr->vertices)
        vabd_ptr->vertices = m_malloc(8 * sizeof(float));
    else
        vabd_ptr->vertices = m_realloc(vabd_ptr->vertices, vabd_ptr->vertices_number * sizeof(float));

    if (NULL == vabd_ptr->txd_vertices)
        vabd_ptr->txd_vertices = m_malloc(vabd_ptr->txd_vertices_number * sizeof(float));
    else
        vabd_ptr->txd_vertices = m_realloc(vabd_ptr->txd_vertices, vabd_ptr->txd_vertices_number * sizeof(float));

    if (NULL == vabd_ptr->indices)
        vabd_ptr->indices = m_malloc(vabd_ptr->indices_number * sizeof(unsigned int));
    else
        vabd_ptr->indices = m_realloc(vabd_ptr->indices, vabd_ptr->indices_number * sizeof(unsigned int));


    for (int i = 0; i < 8; i++)
    {
        vabd_ptr->vertices[i + vabd_ptr->vertices_number - 8] = vertices[i];
        vabd_ptr->txd_vertices[i + vabd_ptr->txd_vertices_number - 8] = txd_vertices[i];
    }

    vabd_ptr->indices[vabd_ptr->indices_number - 6] = vabd_ptr->indices_offset + 0;
    vabd_ptr->indices[vabd_ptr->indices_number - 5] = vabd_ptr->indices_offset + 1;
    vabd_ptr->indices[vabd_ptr->indices_number - 4] = vabd_ptr->indices_offset + 3;
    vabd_ptr->indices[vabd_ptr->indices_number - 3] = vabd_ptr->indices_offset + 1;
    vabd_ptr->indices[vabd_ptr->indices_number - 2] = vabd_ptr->indices_offset + 2;
    vabd_ptr->indices[vabd_ptr->indices_number - 1] = vabd_ptr->indices_offset + 3;
    vabd_ptr->indices_offset += 4;
    return vabd_ptr->indices_number - 6;
}



void va_build(unsigned int va_idx)
{
    if (NULL == _va_to_build)
        return;
    stVaBuildData* vabd_ptr = map_search(_va_to_build, va_idx);
    if (NULL == vabd_ptr)
        return;

    /* Generate a buffer object to store the positions of the vertices */
    glGenBuffers(1, &vabd_ptr->vertex_buffer);

    /* Generate a buffer object to store the texture coordinates */
    glGenBuffers(1, &vabd_ptr->txd_vertex_buffer);

    /* Generate a buffer object to store the vertex indices */
    glGenBuffers(1, &vabd_ptr->indices_buffer);

    /* Set '_vertex_array' as the current vertex array object */
    glBindVertexArray(vabd_ptr->vertex_array);

    /* Bind '_vertex_buffer' to  GL_ARRAY_BUFFER. All following calls to
       GL_ARRAY_BUFFER will calls to GL_ARRAY_BUFFER will refer to this
       '_vertex_buffer' object. */
    glBindBuffer(GL_ARRAY_BUFFER, vabd_ptr->vertex_buffer);

    /* Put data from 'vertices' into GL_ARRAY_BUFFER
       (i.e. into '_vertex_buffer') */
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vabd_ptr->vertices_number, vabd_ptr->vertices, GL_STATIC_DRAW);

    /* Bind '_txd_vertex_buffer' to GL_ARRAY_BUFFER. All following  calls to
       GL_ARRAY_BUFFER will refer to this '_txd_vertex_buffer' object. */
    glBindBuffer(GL_ARRAY_BUFFER, vabd_ptr->txd_vertex_buffer);

    /* Put data from 'txd_vertices' into GL_ARRAY_BUFFER
      (i.e. into '_txd_vertex_buffer' */
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vabd_ptr->txd_vertices_number, vabd_ptr->txd_vertices,
        GL_STATIC_DRAW);

    /* Bind '_indices_buffer' to GL_ELEMENT_ARRAY_BUFFER. All following calls to
       GL_ELEMENT_ARRAY_BUFFER will refer to this 'indices_buffer' object. */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vabd_ptr->indices_buffer);

    /* Put data from 'indices' into GL_ELEMENT_ARRAY_BUFFER
       (i.e. into '_indices_buffer') */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vabd_ptr->indices_number, vabd_ptr->indices,
        GL_STATIC_DRAW);

    /* Bind '_vertex_buffer' to '_vertex_array' at index 0 */
    glBindVertexBuffer(0, vabd_ptr->vertex_buffer, 0, sizeof(GLfloat) * 2);

    /* Bind '_txd_vertex_buffer' to  '_vertex_array' at index 0 */
    glBindVertexBuffer(1, vabd_ptr->txd_vertex_buffer, 0, sizeof(GLfloat) * 2);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    /* '_vertex_buffer' and '_txd_vertex_buffer' can be unbound since they are
        bound to '_vertex_array' as the vertex attributes at indices 0 and 1 */
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    m_free(vabd_ptr->vertices);
    m_free(vabd_ptr->txd_vertices);
    m_free(vabd_ptr->indices);

}


void va_destroy(unsigned int va_idx)
{
    if (NULL == _va_to_build)
        return;

    stVaBuildData* vabd_ptr = map_search(_va_to_build, va_idx);

    if (vabd_ptr != NULL)
    {
        glDeleteVertexArrays(1, &vabd_ptr->vertex_array);
        glDeleteBuffers(1, &vabd_ptr->vertex_buffer);
        glDeleteBuffers(1, &vabd_ptr->txd_vertex_buffer);
        glDeleteBuffers(1, &vabd_ptr->indices_buffer);
        m_free(vabd_ptr);
    }

    map_destroy(_va_to_build);
    _va_to_build = NULL;
}
