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



/** @functions  ------------------------------------------------------------**/

#ifndef TEST_RUN

/**-----------------------------------------------------------------------------
; @func main
;
; @brief
;   Entry point.
;
-----------------------------------------------------------------------------**/
int main(int argc, char* argv[], char* envp[])
{

    return 0;
}

#endif



/**@tests--------------------------------------------------------------------**/
//#define TEST_RUN
//#define MAIN_TEST

#ifdef TEST_RUN
#ifdef MAIN_TEST

#include "test.h"


/**-----------------------------------------------------------------------------
; @unit_test
;
; @brief
;   Verifies that the 'determine_parent_directory' function returns correct
;   values, does not change the state of the incoming variable and does not hang
;   the program when called with invalid/NULL arguments.
-----------------------------------------------------------------------------**/
TEST_BEGIN(test_main)
{
    EXPECT(1, 1);
    EXPECT_NULL(NULL);
    EXPECT_NOT_NULL(123);
    EXPECT_NULL(5);

    TEST_END
}


RUN_TESTS
(
    test_main
)


#endif
#endif