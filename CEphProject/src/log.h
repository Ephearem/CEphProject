/**-----------------------------------------------------------------------------
; @file log.h
;
; @brief
;   The file contains the declaration of functions, macros and data types
;   required to implement the logic that is executed when any error, warning or
;   log message occurs.
;
; @date   October 2021
; @author Eph
;
; // TODO: Log to file. Module output, lines. Termination of the program.
;
-----------------------------------------------------------------------------**/



#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define LOG_MSG(...) printf("LOG: " __VA_ARGS__); printf("\n");
#define LOG_WARNING(...) printf("WARNING: " __VA_ARGS__); printf("\n");
#define LOG_ERROR(...) printf("ERROR: " __VA_ARGS__); printf("\n");

#define GL_CALL(expression) \
do \
{ \
    expression; \
    GLenum result = glGetError(); \
    if(result != GL_NO_ERROR) \
    { \
        LOG_ERROR("Expression: %s\n\tError: %d\n\tFile: %s\n\tLine: %d", #expression,  result, __FILE__, __LINE__); \
    } \
}while (0)

#endif /* LOG_H */
