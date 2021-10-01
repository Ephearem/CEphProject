/**-----------------------------------------------------------------------------
; @file main.c
;
; @brief
;   Contains the entry point to the program.
;
; @date   September 2021
; @author Eph
;
-----------------------------------------------------------------------------**/

#include "core/window.h"
#include "core/loop.h"



/** @functions  ------------------------------------------------------------**/

/**-----------------------------------------------------------------------------
; @func loop_iteration_callback
;
; @brief
;   This function is called at every tick of the main loop.
;
-----------------------------------------------------------------------------**/
void loop_iteration_callback(void)
{

}


/**-----------------------------------------------------------------------------
; @func main
;
; @brief
;   Entry point.
;
-----------------------------------------------------------------------------**/
#ifndef TEST_RUN
int main(int argc, char* argv[], char* envp[])
{
    init_window("CEphProject", 800, 600, 0, 0);
    start_loop(loop_iteration_callback);

    return 0;
}
#endif /* !TEST_RUN */
