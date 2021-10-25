/**-----------------------------------------------------------------------------
; @file texture_builder.c
;
; @brief
;   The file implements the functionality of the 'texture_builder' module
;
;   tb - texture builder
;   abd - array build data
;   lbd - layer build data
;   tbd - texture build data
;
; @date   October 2021
; @author Eph
;
-----------------------------------------------------------------------------**/



/** @includes ----------------------------------------------------------------*/
#include <string.h> /* memcpy */

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture_builder.h"
#include "square.h"
#include "../image.h"
#include "../../memory.h"
#include "../../../containers/list.h"
#include "../../../containers/map.h"
#include "../../../log.h"



/** @types -------------------------------------------------------------------*/

/* Information about the texture to be created */
typedef struct
{
    /* Path to an image where the texture is located */
    const char* image_path;

    /* Offset (in pixels) from the upper left corner of the image where the
        texture is located. */
    int subimg_x;
    int subimg_y;

    /* The size (in pixels) of the portion of the image to use as the texture */
    int subimg_w;
    int subimg_h;

    /* The memory address where the texture information will be written */
    stTexture* target;

    /* Offset (in pixels) at which the texture will be added to the layer */
    int layer_offset_x;
    int layer_offset_y;
}stTextureBuildData;


/* Information about the layer to be created */
typedef struct
{
    stSquare* square;                   /* Used/unused layer pixels           */
    list* textures;                     /* List of 'stTextureBuildData'       */
}stLayerBuildData;


/* Information about the OpenGL texture 2d array to be created */
typedef struct
{
    GLenum unit;
    list* layers;                       /* List of 'stLayerBuildData objects  */

}stArrayBuildData;



/** @static_data -------------------------------------------------------------*/

/* Stores information about all textures to be built */
static list* _textures_to_build = NULL; /* List of 'stTextureBuildData'       */
static map* _texture_groups_to_build = NULL;
static list* _arrays_to_build = NULL;   /* List of 'stArrayBuildData'         */
static list* _group_indices = NULL;     /* List of 'int'                      */

/* Stores pointers to created textures. Used to remove them from video
   memory and CPU */
static list* _created_textures = NULL;  /* List of 'stTexture'                */



/** @internal_prototypes -----------------------------------------------------*/
static unsigned int _create_texture_2d_array(unsigned int unit,
    int width, int height, int depth);
static void _cleanup_build_data(void);
static void _fit_texture(stTextureBuildData* tbd);
static void _fit_texture_group(list* group_textures);
static int _try_add_texture_on_layer(stTextureBuildData* tbd_what,
    stLayerBuildData* lbd_where);
static void _remove_texture_from_lyer(stTextureBuildData* tbd,
    stLayerBuildData* lbd);
static stLayerBuildData* _create_layer_bd(void);
static stArrayBuildData* _create_array_bd(void);
static stTexture* _load_texture_into_texture_2d_array(
    unsigned int array_id,
    unsigned int unit,
    int z_offset,
    int subimage_x_offset,              // TODO: Use cglm.
    int subimage_y_offset,              // TODO: Use cglm.
    int subimage_width,                 // TODO: Use cglm.
    int subimage_height,                // TODO: Use cglm.
    int image_x_offset,                 // TODO: Use cglm.
    int image_y_offset,                 // TODO: Use cglm.
    unsigned char const* image_bytes,
    int image_width,                    // TODO: Use cglm.
    int image_height,                   // TODO: Use cglm.
    int image_channels_count);
static void _calculate_array_size(stArrayBuildData* tabd,
    int* out_w, int* out_h);
static int _get_max_texture_image_units(void);
static int _get_max_3d_texture_size(void);
static int _get_max_array_texture_layers(void);
static int _get_texture_2d_array_width(unsigned int id);
static int _get_texture_2d_array_depth(unsigned int id);
static int _get_texture_2d_array_height(unsigned int id);



/** @functions ---------------------------------------------------------------*/

/**-----------------------------------------------------------------------------
; @func tb_add_texture
;
; @brief
;   Saves data for texture creation. Returns the address where the information
;   about the texture created based on the received data will be located.
;   Information at this address will be filled only after calling the 'tb_build'
;   function.
;
; @params
;   group_idx   | Texture group number. Textures with the same group are
;               | guaranteed to be placed on the same layer.
;   image_path  | Path to the image from which (or from a part of which) the
;               | texture should be created.
;   subimg_x    | x-offset (in pixels) from the upper left corner of the image.
;   subimg_y    | y-offset (in pixels) from the upper left corner of the image.
;   subimg_w    | Width (in pixels) of an image (or a part of an image) that
;               | should be used to create the texture.
;   subimg_h    | Height (in pixels) of an image (or a part of an image) that
;               | should be used to create the texture.
; @return
;   stTexture * | Pointer to a memory area that will store information about the
;               | texture after its creation (i.e. after calling the 'tb_build'
;               | function).
;
-----------------------------------------------------------------------------**/
stTexture* tb_add_texture(int group_idx, const char* image_path, int subimg_x,
    int subimg_y, int subimg_w, int subimg_h)
{
    extern list* _textures_to_build;
    extern map* _texture_groups_to_build;
    extern list* _group_indices;

    stTextureBuildData* texture_build_data_ptr = m_malloc(sizeof(stTextureBuildData));
    texture_build_data_ptr->image_path = image_path;
    texture_build_data_ptr->subimg_x = subimg_x;
    texture_build_data_ptr->subimg_y = subimg_y;
    texture_build_data_ptr->subimg_w = subimg_w;
    texture_build_data_ptr->subimg_h = subimg_h;
    texture_build_data_ptr->target = m_calloc(1, sizeof(stTexture));
    texture_build_data_ptr->target->texture_info_ptr = m_calloc(1, sizeof(stTextureInfo));

    texture_build_data_ptr->layer_offset_x = -1; /* Will be filled in build() */
    texture_build_data_ptr->layer_offset_y = -1; /* Will be filled in build() */

    if (group_idx == TB_NO_GROUP)
    {
        if(NULL == _textures_to_build)
            _textures_to_build = list_create();
        list_push(_textures_to_build, texture_build_data_ptr);
    }
    else
    {
        if (NULL == _texture_groups_to_build)
            _texture_groups_to_build = map_create();

        list* cur_group_textures = map_search(_texture_groups_to_build, group_idx);
        if (NULL == cur_group_textures)
        {
            cur_group_textures = list_create();
            if (NULL == _group_indices)
                _group_indices = list_create();
            list_push(_group_indices, (void*)group_idx);
            map_insert(_texture_groups_to_build, group_idx, cur_group_textures);
        }

        list_push(cur_group_textures, texture_build_data_ptr);
    }
    return texture_build_data_ptr->target;
}


/**-----------------------------------------------------------------------------
; @func tb_build
;
; @brief
;   Creates and places textures on certain OpenGL texture 2d arrays, units and
;   layers.
;
-----------------------------------------------------------------------------**/
void tb_build(void)
{
    extern list* _arrays_to_build;
    extern list* _textures_to_build;
    extern map* _texture_groups_to_build;
    extern list* _group_indices;
    extern list* _created_textures;

    /* Remove from video memory textures created during the previous call to the
       'tb_build' function */
    tb_destroy();

    _arrays_to_build = list_create();


    /* Find free space (array and layer) for the current texture */
    if (_textures_to_build != NULL)
    {
        for (list_node* tbd_node = _textures_to_build->nodes; tbd_node != NULL; tbd_node = tbd_node->next)
        {
            stTextureBuildData* tbd = tbd_node->data;
            _fit_texture(tbd);
        }
    }

    /* Find free space (array and layer) for the current texture group */
    if (_texture_groups_to_build != NULL)
    {
        for (list_node* group_index_node = _group_indices->nodes; group_index_node != NULL; group_index_node = group_index_node->next)
        {
            int group_index = (int)(group_index_node->data);
            _fit_texture_group(map_search(_texture_groups_to_build, group_index));
        }
    }
   
    for (list_node* abd_node = _arrays_to_build->nodes; abd_node != NULL; abd_node = abd_node->next)
    {
        stArrayBuildData* abd = abd_node->data;

        int array_w = -1;
        int array_h = -1;

        /* Calculate required array size */
        _calculate_array_size(abd, &array_w, &array_h);
        int array_z = list_get_size(abd->layers);

        unsigned int texture_2d_array = _create_texture_2d_array(abd->unit, array_w, array_h, array_z);

        int cur_z_offset = 0;

        for (list_node* lbd_node = abd->layers->nodes; lbd_node != NULL; lbd_node = lbd_node->next)
        {
            stLayerBuildData* lbd = lbd_node->data;

            for (list_node* tbd_node = lbd->textures->nodes; tbd_node != NULL; tbd_node = tbd_node->next)
            {
                stTextureBuildData* tbd = tbd_node->data;

                // TODO: In order not to open and close the same image several times,
                //       create a tree in which the key is the path to the image and
                //       process the textures in the correct order.
                const stImage* img = load_image(tbd->image_path);
                stTexture* loaded_txd = _load_texture_into_texture_2d_array(
                    texture_2d_array, abd->unit, cur_z_offset,
                    tbd->layer_offset_x, tbd->layer_offset_y,
                    tbd->subimg_w, tbd->subimg_h,
                    tbd->subimg_x, tbd->subimg_y,
                    img->data_ptr,
                    img->width, img->height,
                    img->channels_count);
                free_image(img);

                loaded_txd->texture_info_ptr->unit -= GL_TEXTURE0;

                memcpy(tbd->target->vertices, loaded_txd->vertices, sizeof(float) * 8);
                memcpy(tbd->target->texture_info_ptr, loaded_txd->texture_info_ptr, sizeof(stTextureInfo));
                m_free(loaded_txd->texture_info_ptr);
                m_free(loaded_txd);
                
                /* Save the address of the created texture */
                if (NULL == _created_textures)
                    _created_textures = list_create();
                list_push(_created_textures, tbd->target);
            }
            cur_z_offset++;
        }
    }

    _cleanup_build_data();
}


/**-----------------------------------------------------------------------------
; @func tb_destroy
;
; @brief
;   Completely removes textures created by the last call to the 'tb_buIld'
;   function:
;     - Removes created textures (arrays of 2d textures) from video memory.
;     - Frees memory allocated for each 'stTexture' object created in the
;       'tb_create' function (all pointers that were returned by this function
;       become invalid).
;
-----------------------------------------------------------------------------**/
void tb_destroy(void)
{
    extern list* _created_textures;

    if (NULL == _created_textures)
        return;

    for (list_node* texture_node = _created_textures->nodes;
        texture_node != NULL;
        texture_node = texture_node->next)
    {
        stTexture* texture_ptr = texture_node->data;
        GL_CALL(glDeleteTextures(1, &(texture_ptr->texture_info_ptr->array_id)));
        m_free(texture_ptr->texture_info_ptr);
        m_free(texture_ptr);
    }
    list_destroy(_created_textures);
    _created_textures = NULL;
}


static unsigned int _create_texture_2d_array(unsigned int unit,
    int width, int height, int depth)
{
    unsigned int texture_2d_array = 0;

    GLenum max_texture_image_units = (GLenum)_get_max_texture_image_units();
    if ((unit - GL_TEXTURE0) >= max_texture_image_units)
    {
        LOG_ERROR("Unable to create texture 2d array. Unit number [%d] is "
            "greater than the maximum allowed on this device. Valid unit "
            "range: [0...%d].",
            unit - GL_TEXTURE0, max_texture_image_units - 1);
        return 0;
    }

    int max_3d_texture_size = _get_max_3d_texture_size();
    if (width > max_3d_texture_size || height > max_3d_texture_size)
    {
        LOG_ERROR("Unable to create texture 2d array. The maximum supported "
            "texture image size [%d] has been exceeded.", max_3d_texture_size);
        return 0;
    }

    int max_array_texture_layers = _get_max_array_texture_layers();
    if (depth > max_array_texture_layers)
    {
        LOG_ERROR("Unable to create texture 2d array. The maximum number of "
            "texture layers [%d] has been exceeded.", max_array_texture_layers);
        return 0;
    }

    /* Save the currently activated texture unit */
    int used_unit = 0;
    GL_CALL(glGetIntegerv(GL_ACTIVE_TEXTURE, &used_unit));

    /* Save the currently bound texture 2d array */
    //int bound_texture = 0;
    //GL_CALL(glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &bound_texture));

    GL_CALL(glActiveTexture(unit));

    /* Generate a 2d texture array object */
    GL_CALL(glGenTextures(1, &texture_2d_array));

    /* Set 'texture_2d_array' as the current vertex array object */
    GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, texture_2d_array));

    GL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CALL(glTexImage3D(
        GL_TEXTURE_2D_ARRAY,            /* Target to which the texture is     */
                                        /* bound                              */
        0,                              /* Level                              */
        GL_RGBA8,                       /* Internal format                    */
        width,                          /* Width of the 2d texture array      */
        height,                         /* Heigh of the 2d texture array      */
        depth,                          /* Depth of the 2d texture array      */
        0,                              /* Border, must be 0                  */
        GL_RGBA,                        /* Format of the pixel data           */
        GL_UNSIGNED_BYTE,               /* Data type of the pixel data        */
        NULL));                         /* A pointer to the image data        */

    /* Restore previous used texture unit */
    GL_CALL(glActiveTexture(used_unit));

    /* Restore previous bound texture 2d array */
    //GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, bound_texture));

    return texture_2d_array;
}


/**-----------------------------------------------------------------------------
; @func _cleanup_build_data
;
; @brief
;   Zeroes pointers and frees memory that was used to create textures and
;   distribute them across OpenGL texture 2d arrays and layers.
;
-----------------------------------------------------------------------------**/
static void _cleanup_build_data(void)
{
    extern list* _arrays_to_build;
    extern list* _textures_to_build;
    extern map* _texture_groups_to_build;
    extern list* _group_indices;

    for (list_node* abd_node = _arrays_to_build->nodes; abd_node != NULL; abd_node = abd_node->next)
    {
        stArrayBuildData* abd = abd_node->data;
        for (list_node* lbd_node = abd->layers->nodes; lbd_node != NULL; lbd_node = lbd_node->next)
        {
            stLayerBuildData* lbd = lbd_node->data;

            for (list_node* tbd_node = lbd->textures->nodes; tbd_node != NULL; tbd_node = tbd_node->next)
            {
                stTextureBuildData* tbd = tbd_node->data;
                // TODO: Uncomment after module 'sprite' implemented.
                //stTexture* t = tbd->target;
                //stTextureInfo* ti = t->texture_info_ptr;
                //glDeleteTextures(1, &ti->array_id);
                //m_free(ti);
                //m_free(t);

                m_free(tbd);
            }
            list_destroy(lbd->textures);
            sq_destroy(lbd->square);
            m_free(lbd);
        }
        list_destroy(abd->layers);
        m_free(abd);
    }
    list_destroy(_arrays_to_build);     // TODO: if(NULL == _arrays_to_build)
    list_destroy(_textures_to_build);   // TODO: if(NULL != _textures_to_build)

    _arrays_to_build = NULL;
    _textures_to_build = NULL;

    if (_texture_groups_to_build != NULL)
    {
        for (list_node* group_index_node = _group_indices->nodes; group_index_node != NULL; group_index_node = group_index_node->next)
        {
            int group_index = (int)(group_index_node->data);
            list_destroy(map_search(_texture_groups_to_build, group_index));
        }
        map_destroy(_texture_groups_to_build);
        list_destroy(_group_indices);
        _texture_groups_to_build = NULL;
        _group_indices = NULL;
    }
}


static void _fit_texture(stTextureBuildData* tbd)
{
    /* For each array */
    for (list_node* abd_node = _arrays_to_build->nodes; abd_node != NULL; abd_node = abd_node->next)
    {
        stArrayBuildData* abd = abd_node->data;
        int iter = 0;
        /* For each layer */
        for (list_node* lbd_node = abd->layers->nodes; lbd_node != NULL; lbd_node = lbd_node->next)
        {
            stLayerBuildData* lbd = lbd_node->data;
            int result = _try_add_texture_on_layer(tbd, lbd);
            if (result == 0)
                return;
        }
    }
    _create_layer_bd(); // TODO: Stupid solution.
    _fit_texture(tbd);  // TODO: Stupid solution.
}


static void _fit_texture_group(list* group_textures)
{
    static int is_new_layer_created = 0;

    /* For each array */
    for (list_node* abd_node = _arrays_to_build->nodes; abd_node != NULL; abd_node = abd_node->next)
    {
        stArrayBuildData* abd = abd_node->data;

        /* For each layer */
        for (list_node* lbd_node = abd->layers->nodes; lbd_node != NULL; lbd_node = lbd_node->next)
        {
            stLayerBuildData* lbd = lbd_node->data;

            /* Try to fit all group textures on it */
            for (list_node* tbd_node = group_textures->nodes; tbd_node != NULL; tbd_node = tbd_node->next)
            {
                stTextureBuildData* tbd = tbd_node->data;

                int result = _try_add_texture_on_layer(tbd, lbd);
                if (result != 0)
                {
                    /* Undo all layer changes */
                    for (list_node* _tbd_node = group_textures->nodes; _tbd_node != tbd_node; _tbd_node = _tbd_node->next)
                    {
                        stTextureBuildData* _tbd = _tbd_node->data;
                        _remove_texture_from_lyer(_tbd, lbd);
                    }
                    /* Try with the next layer */
                    break;
                }
                if (tbd_node->next == NULL)
                    return;
            }
        }
    }

    int result = 0;
    stLayerBuildData* new_lbd = _create_layer_bd();
    /* Try to fit all group textures on it */
    for (list_node* tbd_node = group_textures->nodes; tbd_node != NULL; tbd_node = tbd_node->next)
    {
        stTextureBuildData* tbd = tbd_node->data;

        result = _try_add_texture_on_layer(tbd, new_lbd);
        if (result != 0)
        {
            /* Undo all layer changes */
            for (list_node* _tbd_node = group_textures->nodes; _tbd_node != tbd_node; _tbd_node = _tbd_node->next)
            {
                stTextureBuildData* _tbd = _tbd_node->data;
                _remove_texture_from_lyer(_tbd, new_lbd);
            }
        }
    }

    if (result != 0)
    {
        LOG_ERROR("Can't place group %p on one layer of one unit of one array. Not enough storage.", group_textures);
        // TODO: map_erase(_texture_groups_to_build, group_index).
        // TODO: Remove created layer.
    }
}


static int _try_add_texture_on_layer(stTextureBuildData* tbd_what, stLayerBuildData* lbd_where)
{
    // TOOD: NULL-checks?
    sq_get_free_rect(lbd_where->square, tbd_what->subimg_w, tbd_what->subimg_h,
        &tbd_what->layer_offset_x, &tbd_what->layer_offset_y);
    if ((tbd_what->layer_offset_x != SQ_FAIL) &&
        (tbd_what->layer_offset_y != SQ_FAIL))
    {
        sq_use_rect(
            lbd_where->square,
            tbd_what->layer_offset_x, tbd_what->layer_offset_y,
            tbd_what->subimg_w, tbd_what->subimg_h);
        list_push(lbd_where->textures, tbd_what);
        return 0;
    }
    return -1;
}


static void _remove_texture_from_lyer(stTextureBuildData* tbd, stLayerBuildData* lbd)
{
    for (list_node* lbd_texture = lbd->textures->nodes; lbd_texture != NULL; lbd_texture = lbd_texture->next)
    {
        stTextureBuildData* _tbd = lbd_texture->data;
        if (_tbd == tbd)
        {
            sq_unuse_rect(lbd->square, tbd->layer_offset_x, tbd->layer_offset_y,
                tbd->subimg_w, tbd->subimg_h);
            list_erase(lbd->textures, lbd_texture);
            return;
        }
    }
}


static stLayerBuildData* _create_layer_bd(void)
{
    extern list* _arrays_to_build;

    int max_size = _get_max_3d_texture_size();
    int max_depth = _get_max_array_texture_layers();


    for (list_node* abd_node = _arrays_to_build->nodes; abd_node != NULL; abd_node = abd_node->next)
    {
        stArrayBuildData* abd = abd_node->data;

        if (list_get_size(abd->layers) >= max_depth)
            continue; // TODO: Call '_create_texture_array' function?

        stLayerBuildData* layer = m_calloc(1, sizeof(stLayerBuildData));
        layer->square = sq_create(max_size, max_size);
        layer->textures = list_create(); // TODO: Remove.
        list_push(abd->layers, layer);
        return layer;
    }

    _create_array_bd();        // TODO: Stupid solution.
    return _create_layer_bd(); // TODO: Stupid solution.
}


static stArrayBuildData* _create_array_bd(void)
{
    extern list* _arrays_to_build;

    int arrays_to_build_size = list_get_size(_arrays_to_build);

    if (arrays_to_build_size > _get_max_texture_image_units())
    {
        LOG_ERROR("Unable to create a new 2D texture array. All units are used up.");
        return NULL; // TODO: Implement re-bind arrays on units logic system.
    }

    stArrayBuildData* abd = m_malloc(sizeof(stArrayBuildData));
    abd->unit = GL_TEXTURE0 + arrays_to_build_size;
    abd->layers = list_create(); // TODO: Remove.
    list_push(_arrays_to_build, abd);

    return abd;
}


static stTexture* _load_texture_into_texture_2d_array(
    unsigned int array_id,
    unsigned int unit,
    int z_offset,
    int subimage_x_offset,              // TODO: Use cglm.
    int subimage_y_offset,              // TODO: Use cglm.
    int subimage_width,                 // TODO: Use cglm.
    int subimage_height,                // TODO: Use cglm.
    int image_x_offset,                 // TODO: Use cglm.
    int image_y_offset,                 // TODO: Use cglm.
    unsigned char const* image_bytes,
    int image_width,                    // TODO: Use cglm.
    int image_height,                   // TODO: Use cglm.
    int image_channels_count)
{
    if (glIsTexture(array_id) == GL_FALSE)
    {
        LOG_ERROR("Texture 2d array with id [%d] was not created.", array_id);
        return NULL;
    }

    int texture_array_width = _get_texture_2d_array_width(array_id);
    int texture_array_height = _get_texture_2d_array_height(array_id);

    if (((subimage_x_offset + subimage_width) > texture_array_width) ||
        ((subimage_y_offset + subimage_height) > texture_array_height))
    {
        LOG_ERROR("The [%dx%d] texture cannot fit on the [%dx%d] array layer "
            "at the [%dx%d] offset. Going beyond the boundaries of the layer.",
            subimage_width, subimage_height,
            texture_array_width, texture_array_height,
            subimage_x_offset, subimage_y_offset);
        return NULL;
    }

    int texture_array_depth = _get_texture_2d_array_depth(array_id);
    if (z_offset >= texture_array_depth)
    {
        LOG_ERROR("The texture cannot be placed on layer with index [%d] "
            "because there are only [%d] layers in the texture array.",
            z_offset, texture_array_depth);
        return NULL;
    }

    GLenum format = 0;
    switch (image_channels_count)
    {
    case 4:                             /* If 4 bytes per pixel               */
        format = GL_RGBA;               /* It's RGBA                          */
        break;
    case 3:                             /* if 3 bytes per pixel               */
        format = GL_RGB;                /* it's RGB                           */
        break;
    default:                            /* Otherwise, log an error            */
        // TODO: Provide functionality for other formats.
        LOG_ERROR("Undefined image format.");
        return NULL;
    }

    /* Save the currently activated texture unit */
    int used_unit = 0;
    GL_CALL(glGetIntegerv(GL_ACTIVE_TEXTURE, &used_unit));

    /* Save the currently bound texture 2d array */
    //int bound_texture = 0;
    //glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &bound_texture);

    GL_CALL(glActiveTexture(unit));
    GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, array_id));

    GL_CALL(glPixelStorei(GL_UNPACK_ROW_LENGTH, image_width));
                                        /* The full width of the image from   */
                                        /* which the texture is created       */
    GL_CALL(glPixelStorei(GL_UNPACK_SKIP_PIXELS, image_x_offset));
                                        /* Subimage x-offset (from the        */
                                        /* beginning of the image).           */
    GL_CALL(glPixelStorei(GL_UNPACK_SKIP_ROWS, image_height - image_y_offset
        - subimage_height));            /* Subimage y-offset (from the        */
                                        /* beginning of the image).           */

    GL_CALL(glTexSubImage3D(
        GL_TEXTURE_2D_ARRAY,            /* Target to which the texture is     */
                                        /* bound                              */
        0,                              /* Level-of-detail. 0 - base image    */
        subimage_x_offset,              /* X-offset within the texture array  */
        subimage_y_offset,              /* Y offset within the texture array  */

                                        /* The y-offset is calculated from    */
                                        /* the bottom of the texture. For it  */
                                        /* to be considered from the top,     */
                                        /* need to swap the vertices related  */
                                        /* to the texture in the 'vertices'   */
                                        /* array.                             */
        // TODO: Check out the comment posted above because I don't remember
        //       exactly :)

        z_offset,                       /* Z offset (layer)                   */
        subimage_width,                 /* Width of the texture subimage      */
        subimage_height,                /* Height of the texture subimage     */
        1,                              /* Depth of the texture subimage      */
        format,                         /* Format of the pixel data           */
        GL_UNSIGNED_BYTE,               /* Data type of the pixel data        */
        (const void*)image_bytes));     /* Image pixels data pointer          */

    /* Restore previous used texture unit */
    GL_CALL(glActiveTexture(used_unit));

    /* Restore previous bound texture 2d array */
    //GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, bound_texture));

    stTexture* texture_ptr = m_malloc(sizeof(stTexture));
    stTextureInfo* texture_info = m_malloc(sizeof(stTextureInfo));
    texture_ptr->texture_info_ptr = texture_info;
    if (NULL == texture_ptr)
        return NULL;

    /* Calculate the size and coordinates of the texture in relation to the
       dimensions of the layer. */
    float x = (float)subimage_x_offset / texture_array_width;
    float y = (float)subimage_y_offset / texture_array_height;
    float w = (float)subimage_width / texture_array_width;
    float h = (float)subimage_height / texture_array_height;

    /* Construct texture vertices based on the calculated coordinates */
    texture_ptr->vertices[0] = x + w;   /* Top right                          */
    texture_ptr->vertices[1] = y + h;
    texture_ptr->vertices[2] = x + w;   /* Bottom right                       */
    texture_ptr->vertices[3] = y;
    texture_ptr->vertices[4] = x;       /* Bottom left                        */
    texture_ptr->vertices[5] = y;
    texture_ptr->vertices[6] = x;       /* Top left                           */
    texture_ptr->vertices[7] = y + h;

    texture_ptr->texture_info_ptr->array_id = array_id;
    texture_ptr->texture_info_ptr->unit = unit;
    texture_ptr->texture_info_ptr->z_offset = z_offset;
    return texture_ptr;
}


static void _calculate_array_size(stArrayBuildData* tabd,
    int* out_w, int* out_h)
{
    *out_w = -1;
    *out_h = -1;

    for (list_node* lbd_node = tabd->layers->nodes; lbd_node != NULL; lbd_node = lbd_node->next)
    {
        stLayerBuildData* lbd = lbd_node->data;

        int used_x = -1;
        int used_y = -1;
        sq_get_used_rect(lbd->square, &used_x, &used_y);
        if (used_x > *out_w)
            *out_w = used_x;
        if (used_y > *out_h)
            *out_h = used_y;
    }
}


static int _get_max_texture_image_units(void)
{
    static int res = -1;
    if (res != -1)
        return res;

    /* Get the maximum supported texture image units that can be used to access
       texture maps from the fragment shader */
    GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &res));
    return res;
}


static int _get_max_3d_texture_size(void)
{
    static int res = -1;
    if (res != -1)
        return res;

    /* Get the maximum supported texture image size */
    GL_CALL(glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &res));
    return res;
}


static int _get_max_array_texture_layers(void)
{
    static int res = -1;
    if (res != -1)
        return res;

    /* Get the maximum supported texture 2d array depth */
    GL_CALL(glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &res));
    return res;
}


static int _get_texture_2d_array_width(unsigned int id)
{
    int res = 0;

    /* Save the currently bound texture 2d array */
    int bound_texture = 0;
    GL_CALL(glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &bound_texture));

    GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, id));
    GL_CALL(glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_WIDTH, &res));

    /* Restore previous bound texture 2d array */
    GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, bound_texture));

    return res;
}


static int _get_texture_2d_array_height(unsigned int id)
{
    int res = 0;

    /* Save the currently bound texture 2d array */
    int bound_texture = 0;
    GL_CALL(glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &bound_texture));

    GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, id));
    GL_CALL(glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_HEIGHT, &res));

    /* Restore previous bound texture 2d array */
    GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, bound_texture));

    return res;
}


static int _get_texture_2d_array_depth(unsigned int id)
{
    int res = 0;

    /* Save the currently bound texture 2d array */
    int bound_texture = 0;
    GL_CALL(glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &bound_texture));

    GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, id));
    GL_CALL(glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_DEPTH, &res));

    /* Restore previous bound texture 2d array */
    GL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, bound_texture));

    return res;
}
