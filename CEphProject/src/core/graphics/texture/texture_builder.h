/**-----------------------------------------------------------------------------
; @file texture_builder.h
;
; @brief
;   This module implements the logic for creating and placing textures in video
;   memory on certain OpenGL texture 2d arrays, units and layers.
;
; @usage:
;   - specify images (or parts of them) that should be used as textures using
;     the 'tb_add_texture' function;
;   - load this images into video memory using the 'tb_build' function;
;   - remove the created textures after use using the 'tb_destroy' function.
;     This function will remove all 2d texture arrays created with
;     'tb_add_texture' + 'tb_build' from video memory and free their associated
;     data from RAM.
;
; @notes:
;   Each texture has an OpenGL texture id, texture 2d array, texture unit and
;   texture 2d array z-offset. There are situations when for several textures
;   all these values must be the same (for example, if several dozen textures
;   are used to draw the map, it would be logical to place them all on one layer
;   so that the map can be drawn with one call of the corresponding OpenGL
;   function). To solve this problem, the 'group_idx' argument is implemented in
;   the 'tb_add_texture' function. All textures with the same 'group_idx', not
;   equal to 'TB_NO_GROUP', are guaranteed to be placed on the same layer of the
;   same unit of the same 2d texture array (provided that the graphics card has
;   the necessary resource for this).
;
;   The data pointed to by the pointer returned from 'tb_add_texture' becomes
;   valid only after calling the 'tb_build' function.
;
;   After calling 'tb_destroy', all 'stTexture*' returned from 'tb_add_texture'
;   become invalid. 
;
; @date   October 2021
; @author Eph
;
-----------------------------------------------------------------------------**/



#ifndef TEXTURE_BUILDER_H
#define TEXTURE_BUILDER_H



#define TB_NO_GROUP 0

/** @types -------------------------------------------------------------------*/

typedef struct
{
    int array_id; // TODO: Remove this field?
    unsigned int unit;
    int z_offset;
}stTextureInfo;


typedef struct
{
    /* It should be a separate object, because after loading textures into video
       memory, information about vertices ('vertices' array) should be removed
       as unnecessary, but the other fields will be used for rendering. */
    stTextureInfo* texture_info_ptr;
    // TODO: After implementing the 'sprite' module:
    //          1. Use the 'stTextureInfo' object instead of a pointer to it.
    //          2. At the stage of sprite creation - copy this 'stTextureInfo'
    //             object into the memory area next to the sprite (this will
    //             increase the rendering speed, since both texture information
    //             and sprite information will be accessed from approximately
    //             the same place).

    float vertices[8];
}stTexture;



stTexture* tb_add_texture(int group_idx,  const char* image_path, int subimg_x,
    int subimg_y, int subimg_w, int subimg_h);

void tb_build(void);

void tb_destroy(void);



#endif /* !TEXTURE_BUILDER_H */
