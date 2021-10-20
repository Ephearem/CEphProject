/**-----------------------------------------------------------------------------
; @file vertex_array.h
;
; @brief
;   The module is implemented to simplify the process of creating and filling
;   vertex arrays with data.
;
; @usage
;   - create an array of vertices using the 'va_create' function;
;   - create a shape in this array using the 'va_shape_create' function;
;   - add primitives to this shape using the 'va_shape_add_textured_rect' func.
;       At this stage, it is possible to only add a textured quadrilateral.
;       Hopefully the functionality will be improved (for example, adding a
;       circle, triangle, 3d objects, etc.);
;   - build an array of vertices (load all primitives of all shapes into video
;     memory) using the 'va_build' function;
;   - delete the created vertex array after use using the 'va_destroy' function.
;     This function will clear the video memory and delete the information about
;     the created shapes from the main memory.
;
; @notes
;   The data pointed to by the pointer returned from 'va_shape_create' becomes
;   valid only after calling the 'va_build' function.
;
;   After calling 'va_destroy', all 'stIndicesInfo*' returned from
;   'va_shape_create' become invalid.
; 
; @date   October 2021
; @author Eph
;
-----------------------------------------------------------------------------**/

#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H



/** @types -------------------------------------------------------------------*/

/* The structure contains a minimum set of data required to render anything
   using an index buffer ('GL_ELEMENT_ARRAY_BUFFER'). In fact, an object of this
   structure stores arguments for 'glDrawElements' function. */
typedef struct stIndicesInfo
{
    // TODO: Add vertex array idx.
    /* const */ unsigned int mode;      /* Vertices connection mode           */
    /* const */ unsigned int count;     /* Number of elements to be rendered  */
    /* const */ void* offset;           /* Offset to the first index of the   */
                                        /* shape(s)                           */
}stIndicesInfo;



unsigned int va_create(void);
stIndicesInfo* va_shape_create(unsigned int va_idx);
void va_shape_add_textured_rect(unsigned int va_idx, stIndicesInfo* shape,
    float* vertices, float* txd_vertices);
void va_build(unsigned int va_idx);
void va_destroy(unsigned int va_idx);



#endif /* !VERTEX_ARRAY_H */
