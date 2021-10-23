#include "../../memory.h"

#include "square.h"
#include "../../../log.h"



#define SQ_FREE 0x00
#define SQ_USED 0xFF


typedef struct stSquare
{
    int w;
    int h;
    unsigned char** bytes;
}stSquare;


stSquare* sq_create(int w, int h)
{
    stSquare* sq = m_calloc(1, sizeof(stSquare));
    if (NULL == sq)
    {
        LOG_ERROR("// TODO:");
        return NULL;
    }
    sq->w = w;
    sq->h = h;
    sq->bytes = m_calloc(w * h, sizeof(unsigned char*));
    if (NULL == sq->bytes)
    {
        LOG_ERROR("// TODO:");
        return NULL;
    }
    return sq;
}


void sq_destroy(stSquare* sq)
{
    m_free(sq->bytes);
    m_free((void*)sq);
}


/**-----------------------------------------------------------------------------
; @func sq_use_rect
;
; @brief
;   Places a rectangle in a square at specified coordinates.
;
; @params
;   sq  | Square.
;   x   | X-position of the left-top corner of the rectangle to be placed.
;   y   | Y-position of the left-top corner of the rectangle to be placed.
;   w   | Width of the rectangle to be placed.
;   h   | Height of the rectangle to be placed.
;
-----------------------------------------------------------------------------**/
void sq_use_rect(stSquare* sq, int x, int y, int w, int h)
{
    if ((x + w) > sq->w)
    {
        // TODO: Log an error.
        return;
    }
    if ((y + h) > sq->h)
    {
        // TODO: Log an error.
        return;
    }

    for (int Y = y; Y < y + h; Y++) /* For each line */
    {
        for (int X = x; X < x + w; X++) /* For each pixel */
        {
            sq->bytes[Y * sq->w + X] = SQ_USED;
        }
    }
}


/**-----------------------------------------------------------------------------
; @func sq_use_rect
;
; @brief
;   Removes a quadrilateral in a square at specified coordinates.
;
; @params
;   sq  | Square.
;   x   | X-position of the left-top corner of the rectangle to be removed.
;   y   | Y-position of the left-top corner of the rectangle to be removed.
;   w   | Width of the rectangle to be removed.
;   h   | Height of the rectangle to be removed.
;
; // TODO: The logic is the same as in 'sq_use_rect'. Move it into a separate
;          function.
;
-----------------------------------------------------------------------------**/
void sq_unuse_rect(stSquare* sq, int x, int y, int w, int h)
{
    if ((x + w) > sq->w)
    {
        // TODO: Log an error.
        return;
    }
    if ((y + h) > sq->h)
    {
        // TODO: Log an error.
        return;
    }

    for (int Y = y; Y < y + h; Y++) /* For each line */
    {
        for (int X = x; X < x + w; X++) /* For each pixel */
        {
            sq->bytes[Y * sq->w + X] = SQ_FREE;
        }
    }
}


/**-----------------------------------------------------------------------------
; @func sq_get_free_rect
;
; @brief
;   Returns the coordinates at which a rectangle of size ('w', 'h') can be
;   placed so that it does not overlap with already occupied areas of the 'sq'
;   quare.
;
; @params
;   sq      | Square.
;   w       | Width of the rectangle to be placed.
;   h       | Height of the rectangle to be placed.
;   out_x   | X-pos on which a rectangle of size ('w', 'h') can be placed.
;   out_y   | Y-pos on which a rectangle of size ('w', 'h') can be placed.
;
-----------------------------------------------------------------------------**/
void sq_get_free_rect(const stSquare* sq, int w, int h, int* out_x, int* out_y)
{
    if (w > sq->w)
    {
        // TODO: Log an error.
        *out_x = SQ_FAIL;
        *out_y = SQ_FAIL;
        return;
    }
    if (h > sq->h)
    {
        // TODO: Log an error.
        *out_x = SQ_FAIL;
        *out_y = SQ_FAIL;
        return;
    }

        /* For each line */
    for (int Y = 0; Y <= sq->h - h; Y++)
    {
        /* For each pixel */
        for (int X = 0; X <= sq->w - w; X++)
        {
            if (Y > (sq->h - h))
                break;
            if (X > (sq->w - w))
                break;

            for (int YY = 0; YY < h; YY++)
            {
                for (int XX = 0; XX < w; XX++)
                {
                    if (sq->bytes[(Y + YY) * sq->w + X + XX] == SQ_USED)
                    {
                        X += XX;
                        Y += YY;
                        goto l1;
                    }
                }
            }
            *out_x = X;
            *out_y = Y;
            return;
        l1: continue;
        }
    }

    // TODO: Log an error.
    *out_x = SQ_FAIL;
    *out_y = SQ_FAIL;
    return;
}


/**-----------------------------------------------------------------------------
; @func sq_get_used_rect
;
; @brief
;   Returns the size of the smallest rectangle that can fit all rectangles added
;   by the 'sq_use_rect' function.
; @params
;   sq      | Square.
;   out_w   | Used space along the x-axis.
;   out_h   | Used space along the y-axis.
;
-----------------------------------------------------------------------------**/
void sq_get_used_rect(const stSquare* sq, int* out_w, int* out_h)
{
    *out_w = sq->w;
    *out_h = sq->h;

    for (int y = sq->h - 1; y >= 0; y--)
    {
        for (int x = 0; x < sq->w; x++)
        {
            if (sq->bytes[y * sq->w + x] == SQ_USED)
            {
                *out_h = y + 1;
                goto y_completed;
            }
        }
    }

y_completed:
    for (int x = sq->w - 1; x >= 0; x--)
    {
        for (int y = 0; y < sq->h; y++)
        {
            if (sq->bytes[y * sq->w + x] == SQ_USED)
            {
                *out_w = x + 1;
                return;
            }
        }
    }
    return;
}


void sq_dbg_print(const stSquare* sq, int x, int y, int w, int h)
{
    for (int _y = y; _y < (y + h); _y++)
    {
        for (int _x = x; _x < (x + w); _x++)
        {
            if (sq->bytes[_y * sq->w + _x] == SQ_USED)
                printf("*");

            else
                printf(" ");
        }
        printf("\n");
    }
    printf("\n\n");
}
