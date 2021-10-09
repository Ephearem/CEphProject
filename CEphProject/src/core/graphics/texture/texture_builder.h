/**-----------------------------------------------------------------------------
; @file texture_builder.h
;
; @brief
;   This module implements the logic for creating and placing textures in video
;   memory on certain OpenGL texture 2d arrays, units and layers.
;
; @date   October 2021
; @author Eph
;
-----------------------------------------------------------------------------**/



#ifndef TEXTURE_BUILDER_H
#define TEXTURE_BUILDER_H



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



stTexture* tb_add_texture(const char* image_path, int subimg_x, int subimg_y,
    int subimg_w, int subimg_h);


void tb_build(void);



#endif /* !TEXTURE_BUILDER_H */
