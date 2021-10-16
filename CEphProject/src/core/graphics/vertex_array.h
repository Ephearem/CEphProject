/**-----------------------------------------------------------------------------
; @file vertex_array.h
;
; @brief
;   The module is implemented to simplify the process of creating and filling
;   vertex arrays with data.
;
; @date   October 2021
; @author Eph
;
-----------------------------------------------------------------------------**/

#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H



unsigned int va_create(void);

size_t va_add_textured_rect(unsigned int va_idx, float* vertices, float* txd_vertices);

void va_build(unsigned int va_idx);

void va_destroy(unsigned int va_idx);



#endif /* !VERTEX_ARRAY_H */
