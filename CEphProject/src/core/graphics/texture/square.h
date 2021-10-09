/**-----------------------------------------------------------------------------
; @file square.h
;
; @brief
;   This module is used to simulate a 2d area for optimal placement of textures
;   on it.
;
; @date   October 2021
; @author Eph
;
-----------------------------------------------------------------------------**/



#ifndef SQUARE_H
#define SQUARE_H



#define SQ_FAIL (-1)



// TODO: Integrate the module into a texture creation module?


typedef struct stSquare stSquare;


stSquare* sq_create(int w, int h);
void sq_destroy(stSquare* sq);
void sq_use_rect(stSquare* sq, int x, int y, int w, int h);
void sq_unuse_rect(stSquare* sq, int x, int y, int w, int h);
void sq_get_free_rect(const stSquare* sq, int w, int h, int* out_x, int* out_y);
void sq_get_used_rect(const stSquare* sq, int* out_w, int* out_h);
void sq_dbg_print(const stSquare* sq, int x, int y, int w, int h);



#endif /* !SQUARE_H */
