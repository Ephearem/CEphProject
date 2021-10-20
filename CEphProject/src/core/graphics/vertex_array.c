/**-----------------------------------------------------------------------------
; @file vertex_array.c
;
; @brief
;   The file implements the functionality of the 'vertex_array' module.
;
;   va - vertex array
;   vabd - vertex array build data
;   sbd - shape build data
;   ii - indices info
;
; @date   October 2021
; @author Eph
;
-----------------------------------------------------------------------------**/

#include <glad/glad.h>

#include "vertex_array.h"
#include "../../containers/list.h"
#include "../../containers/map.h"
#include "../memory.h"
#include "../../log.h"



#define VERTICES_PER_RECTANGLE 8
#define TEXTURE_VERTICES_PER_RECTANGLE VERTICES_PER_RECTANGLE
#define INDICES_PER_RECTANGLE 6
#define INDICES_USAGE_PER_RECTANGLE 4



/** @types -------------------------------------------------------------------*/

/* Information about created vertex array */
typedef struct stVertexArray
{
    unsigned int vertex_array;          /* OpenGL vertex array object id      */
    unsigned int vertex_buffer;         /* OpenGL GL_ARRAY_BUFFER  object id  */
    unsigned int txd_vertex_buffer;     /* OpenGL GL_ARRAY_BUFFER  object id  */
    unsigned int indices_buffer;        /* OpenGL GL_ELEMENT_ARRAY_BUFFER id  */
    list* ii_list;                      /* List of 'stIndicesInfo'            */
}stVertexArray;


/* Information about vertex array to build */
typedef struct stVaBuildData
{
    stVertexArray* va;
    list* vasbd_list;                   /* List of 'stVaShapeBuildData'       */

}stVaBuildData;


/* Information about a shape to add to the vertex array */
typedef struct stVaShapeBuildData
{
    float* vertices;                    /* Shape vertices                     */
    float* txd_vertices;                /* Shape texture vertices             */
    unsigned int* indices;              /* Shape indices                      */

    int vertices_number;                /* Number of 'vertices' items         */
    int txd_vertices_number;            /* Number of 'txd_vertices' items     */
    int indices_number;                 /* Number of 'indices' items          */

    unsigned int indices_offset;        /* Next free index number             */

    stIndicesInfo* target;              /* The memory address at which
                                        /* information for rendering this     */
                                        /* shape will be written (after       */
                                        /* building)                          */
}stVaShapeBuildData;



/** @static_data -------------------------------------------------------------*/

/* Information about all vertex arrays to be created */
static map* _va_to_build = NULL;        /* Map of 'stVaBuildData'             */

/* Information about all created vertex arrays */
static map* _built_va = NULL;           /* Map of 'stVertexArray'             */



/** @internal_prototypes -----------------------------------------------------*/
static void _destroy_build_data(unsigned int va_idx);



/** @functions ---------------------------------------------------------------*/

unsigned int va_create(void)
{
    extern map* _va_to_build;

    /* Init va-to-build storage if not inited */
    if (NULL == _va_to_build)
        _va_to_build = map_create();

    /* Generate a verex array OpenGL object  */
    unsigned int va_idx = 0;
    GL_CALL(glGenVertexArrays(1, &va_idx));
    // TODO: Return if va_idx <= 0.

    /* Create vertex array build data */
    stVaBuildData* vabd = m_calloc(1, sizeof(stVaBuildData));
    stVertexArray* va = m_calloc(1, sizeof(stVertexArray));
    va->vertex_array = va_idx;
    va->ii_list = list_create();
    vabd->va = va;
    vabd->vasbd_list = list_create();

    /* Store this va data in va-to-build storage*/
    map_insert(_va_to_build, va_idx, vabd);

    return  va_idx;
}


stIndicesInfo* va_shape_create(unsigned int va_idx)
{
    extern map* _va_to_build;

    if (NULL == _va_to_build)
    {
        LOG_ERROR("Vertex array with index %d does not exist.", va_idx);
        return NULL;
    }

    stVaBuildData* vabd = map_search(_va_to_build, va_idx);
    if (NULL == vabd)
    {
        LOG_ERROR("Vertex array with index %d does not exist.", va_idx);
        return NULL;
    }

    stVaShapeBuildData* vasbd = m_calloc(1, sizeof(stVaShapeBuildData));
    vasbd->target = m_calloc(1, sizeof(stIndicesInfo));

    list_push(vabd->vasbd_list, vasbd);

    return vasbd->target;

}


void va_shape_add_textured_rect(unsigned int va_idx, stIndicesInfo* shape,
    float* vertices, float* txd_vertices)
{
    extern map* _va_to_build;

    if (NULL == _va_to_build)
    {
        LOG_ERROR("Vertex array with index %d does not exist.", va_idx);
        return;
    }

    stVaBuildData* vabd = map_search(_va_to_build, va_idx);
    if (NULL == vabd)
    {
        LOG_ERROR("Vertex array with index %d does not exist.", va_idx);
        return;
    }

    stVaShapeBuildData* vabd_sbd = NULL;
    for (list_node* vabd_sbd_node = vabd->vasbd_list->nodes; vabd_sbd_node != NULL; vabd_sbd_node = vabd_sbd_node->next)
    {
        stVaShapeBuildData* cur_vsbd = vabd_sbd_node->data;
        if (cur_vsbd->target == shape)
        {
            vabd_sbd = cur_vsbd;
            break;
        }
    }
    if (NULL == vabd_sbd)
    {
        LOG_ERROR("Vertex array %d does not contain shape '%p'.", va_idx, shape);
        return;
    }

    vabd_sbd->vertices_number += VERTICES_PER_RECTANGLE;
    vabd_sbd->txd_vertices_number += TEXTURE_VERTICES_PER_RECTANGLE;
    vabd_sbd->indices_number += INDICES_PER_RECTANGLE;

    if (NULL == vabd_sbd->vertices)
        vabd_sbd->vertices = m_malloc(vabd_sbd->vertices_number * sizeof(float));
    else
        vabd_sbd->vertices = m_realloc(vabd_sbd->vertices, vabd_sbd->vertices_number * sizeof(float));

    if (NULL == vabd_sbd->txd_vertices)
        vabd_sbd->txd_vertices = m_malloc(vabd_sbd->txd_vertices_number * sizeof(float));
    else
        vabd_sbd->txd_vertices = m_realloc(vabd_sbd->txd_vertices, vabd_sbd->txd_vertices_number * sizeof(float));

    if (NULL == vabd_sbd->indices)
        vabd_sbd->indices = m_malloc(vabd_sbd->indices_number * sizeof(unsigned int));
    else
        vabd_sbd->indices = m_realloc(vabd_sbd->indices, vabd_sbd->indices_number * sizeof(unsigned int));

    for (int i = 0; i < VERTICES_PER_RECTANGLE; i++) // TODO: Use memcpy.
    {
        vabd_sbd->vertices[i + vabd_sbd->vertices_number - VERTICES_PER_RECTANGLE] = vertices[i];
        vabd_sbd->txd_vertices[i + vabd_sbd->txd_vertices_number - TEXTURE_VERTICES_PER_RECTANGLE] = txd_vertices[i];
    }

    vabd_sbd->indices[vabd_sbd->indices_number - INDICES_PER_RECTANGLE + 0] = vabd_sbd->indices_offset + 0;
    vabd_sbd->indices[vabd_sbd->indices_number - INDICES_PER_RECTANGLE + 1] = vabd_sbd->indices_offset + 1;
    vabd_sbd->indices[vabd_sbd->indices_number - INDICES_PER_RECTANGLE + 2] = vabd_sbd->indices_offset + 3;
    vabd_sbd->indices[vabd_sbd->indices_number - INDICES_PER_RECTANGLE + 3] = vabd_sbd->indices_offset + 1;
    vabd_sbd->indices[vabd_sbd->indices_number - INDICES_PER_RECTANGLE + 4] = vabd_sbd->indices_offset + 2;
    vabd_sbd->indices[vabd_sbd->indices_number - INDICES_PER_RECTANGLE + 5] = vabd_sbd->indices_offset + 3;
    vabd_sbd->indices_offset += INDICES_USAGE_PER_RECTANGLE;
}


/**-----------------------------------------------------------------------------
; @func va_build
;
; @brief
;
; // TODO: Implement removing 'stVertexArray' and 'stIndicesInfo' with missing
;          shapes.
-----------------------------------------------------------------------------**/
void va_build(unsigned int va_idx)
{
    extern map* _va_to_build;
    extern map* _built_va;

    if (NULL == _va_to_build)
    {
        LOG_ERROR("Vertex array with index %d does not exist.", va_idx);
        return;
    }

    stVaBuildData* vabd = map_search(_va_to_build, va_idx);
    if (NULL == vabd)
    {
        LOG_ERROR("Vertex array with index %d does not exist.", va_idx);
        return;
    }

    if (map_search(_built_va, va_idx) != NULL)
    {
        LOG_WARNING("Vertex array with index %d has already been built.", va_idx);
        return;
    }

    size_t total_vertices = 0;
    size_t total_txd_vertices = 0;
    size_t total_indices = 0;

    /* Count the number of vertices to add to the vertex array */
    for (list_node* shape_node = vabd->vasbd_list->nodes; shape_node != NULL; shape_node = shape_node->next)
    {
        stVaShapeBuildData* vsbd = shape_node->data;
        total_vertices += vsbd->vertices_number;
        total_txd_vertices += vsbd->txd_vertices_number;
        total_indices += vsbd->indices_number;
    }
    //if ((total_vertices == 0) || (total_txd_vertices == 0) || (total_indices == 0))
    //{
    //
    //}
      // TODO: Remove the next object if an error occurs during the further
    ////       operation of this function.
    //stVertexArray* va = m_calloc(1, sizeof(stVertexArray));
    //va->vertex_array = va_idx;
    //va->shapes_indices_info = list_create();
    //vabd->va = va;
    stVertexArray* va = vabd->va;

    /* Set 'va->vertex_array' as the current vertex array object */
    GL_CALL(glBindVertexArray(va->vertex_array));

    /* Generate a buffer object to store the positions of the vertices */
    GL_CALL(glGenBuffers(1, &va->vertex_buffer));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, va->vertex_buffer));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * total_vertices, NULL, GL_STATIC_DRAW));

    /* Generate a buffer object to store the texture coordinates */
    GL_CALL(glGenBuffers(1, &va->txd_vertex_buffer));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, va->txd_vertex_buffer));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * total_txd_vertices, NULL, GL_STATIC_DRAW));

    /* Generate a buffer object to store the vertex indices */
    GL_CALL(glGenBuffers(1, &va->indices_buffer));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, va->indices_buffer));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * total_indices, NULL, GL_STATIC_DRAW));

    size_t va_vertices_offset = 0;
    size_t va_txd_vertices_offset = 0;
    size_t va_indices_offset = 0;

    unsigned int cur_shape_indices_usage = 0;

    /* For each shape to be added to the vertex array */
    for (list_node* shape_node = vabd->vasbd_list->nodes; shape_node != NULL; shape_node = shape_node->next)
    {
        stVaShapeBuildData* vsbd = shape_node->data;

        for (int i = 0; i < vsbd->indices_number; i++)
            vsbd->indices[i] += cur_shape_indices_usage;

        /* Send vertex information to video memory */
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, va->vertex_buffer));
        GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, va_vertices_offset, sizeof(float) * vsbd->vertices_number, vsbd->vertices));

        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, va->txd_vertex_buffer));
        GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, va_txd_vertices_offset, sizeof(float) * vsbd->txd_vertices_number, vsbd->txd_vertices));

        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, va->indices_buffer));
        GL_CALL(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, va_indices_offset, sizeof(unsigned int) * vsbd->indices_number, vsbd->indices));

        /* Fill in information about vertices */
        vsbd->target->mode = GL_TRIANGLES;
        vsbd->target->count = vsbd->indices_number;
        vsbd->target->offset = (void*)va_indices_offset;
        /* Push it in va's vertices storage */
        list_push(va->ii_list, vsbd->target);

        va_vertices_offset += vsbd->vertices_number * sizeof(float);
        va_txd_vertices_offset += vsbd->txd_vertices_number * sizeof(float);
        va_indices_offset += vsbd->indices_number * sizeof(unsigned int);

        cur_shape_indices_usage += vsbd->indices_offset;
    }

    /* Bind 'va->vertex_buffer' to 'va->vertex_array' at index 0 */
    GL_CALL(glBindVertexBuffer(0, va->vertex_buffer, 0, sizeof(GLfloat) * 2));

    /* Bind 'va->txd_vertex_buffer' to 'va->vertex_array' at index 1 */
    GL_CALL(glBindVertexBuffer(1, va->txd_vertex_buffer, 0, sizeof(GLfloat) * 2));

    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glEnableVertexAttribArray(1));

    /* 'va->vertex_buffer' and 'va->txd_vertex_buffer' can be unbound since they
        are bound to 'va->vertex_array' as the vertex attributes at indices 0
        and 1 */
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    /* Create a built vertex array storage */
    if (NULL == _built_va)
        _built_va = map_create();

    /* Put current vertex array info in the storage */
    map_insert(_built_va, va_idx, va);

    _destroy_build_data(va_idx);
}


/**-----------------------------------------------------------------------------
; @func va_destroy
;
; @brief
;   Completely removes vertex array with index 'va_idx' if one was created using
;   the 'va_create' function:
;     - Removes created vertices, texture vertices and indices buffers, vertices 
;       arrays from video memory.
;     - Frees memory allocated for each 'stIndicesData' object created in the
;       'va_shape_create' function (all pointers that were returned by this
;       function become invalid).
;     - Erases 'stIndicesData' objects related to the removed vertex array from
;       created vertex arrays storage '_built_va'.
;     - Removes the '_built_va' storage if there are no items left in it.
;
-----------------------------------------------------------------------------**/
void va_destroy(unsigned int va_idx)
{
    extern map* _built_va;

    if (NULL == _built_va)
        return;

    stVertexArray* va = map_search(_built_va, va_idx);
    if (NULL == va)
        return;

    /* Remove OpenGL objects */
    GL_CALL(glDeleteVertexArrays(1, &va->vertex_array));
    GL_CALL(glDeleteBuffers(1, &va->vertex_buffer));
    GL_CALL(glDeleteBuffers(1, &va->txd_vertex_buffer));
    GL_CALL(glDeleteBuffers(1, &va->indices_buffer));

    /* Remove indices info for each va's shape */
    for (list_node* vasii_node = va->ii_list->nodes; vasii_node != NULL; vasii_node = vasii_node->next)
    {
        stIndicesInfo* ii = vasii_node->data;
        m_free(ii);
    }

    /* Destroy indices' list */
    list_destroy(va->ii_list);
    
    /* Remove va object */
    m_free(va);

    /* Remove va object ptr from created va's storage */
    map_erase(_built_va, va_idx);

    if (0 == map_get_size(_built_va))
    {
        map_destroy(_built_va);
        _built_va = NULL;
    }
}


/**-----------------------------------------------------------------------------
; @func _destroy_build_data
;
; @brief
;   Zeroes pointers and frees memory that was used to create vertex array with
;   index 'va_idx'.
;   Deletes 'stIndicesInfo' and frees the video memory filled with data related
;   to the 'va_idx' array.
;
-----------------------------------------------------------------------------**/
static void _destroy_build_data(unsigned int va_idx)
{
    extern map* _va_to_build;
    extern map* _built_va;

    if (NULL == _va_to_build)
        return;

    stVaBuildData* vabd = map_search(_va_to_build, va_idx);
    if (NULL == vabd)
        return;

    int is_this_va_built_successfully = 0;

    if (NULL != _built_va)
        if (map_search(_built_va, va_idx) != 0)
            is_this_va_built_successfully = 1;

    /* For each shape build data of current va */
    for (list_node* vasbd_node = vabd->vasbd_list->nodes; vasbd_node != NULL; vasbd_node = vasbd_node->next)
    {
        stVaShapeBuildData* vasbd = vasbd_node->data;

        /* Remove vertices, texture vertices, indices */
        if (vasbd->vertices != NULL)
            m_free(vasbd->vertices);
        if(vasbd->txd_vertices != NULL)
            m_free(vasbd->txd_vertices);
        if (vasbd->indices != NULL)
            m_free(vasbd->indices);

        // TODO: Remove empty shapes.

        m_free(vasbd);
    }
    list_destroy(vabd->vasbd_list);

    /* If this va was not built successfully */
    if (!is_this_va_built_successfully)
    {
        stVertexArray* va = vabd->va;

        /* Remove OpenGL objects */
        GL_CALL(glDeleteVertexArrays(1, &va->vertex_array));
        GL_CALL(glDeleteBuffers(1, &va->vertex_buffer));
        GL_CALL(glDeleteBuffers(1, &va->txd_vertex_buffer));
        GL_CALL(glDeleteBuffers(1, &va->indices_buffer));

        /* Remove indices info for each va's shape */
        for (list_node* vasii_node = va->ii_list->nodes; vasii_node != NULL; vasii_node = vasii_node->next)
        {
            stIndicesInfo* ii = vasii_node->data;
            m_free(ii);
        }

        /* Destroy indices' list */
        list_destroy(va->ii_list);

        /* Remove va object */
        m_free(va);
    }

    m_free(vabd);
    map_erase(_va_to_build, va_idx);

    if(0 == map_get_size(_va_to_build))
    {
        map_destroy(_va_to_build);
        _va_to_build = NULL;
    }
}
