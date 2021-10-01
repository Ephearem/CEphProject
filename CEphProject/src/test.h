/**-----------------------------------------------------------------------------
; @file test.h
;
; @brief
;   The file implements the functionality for writing unit tests.
;
;   Usage:
;   At the end of the .c file of the unit under test:
;   - Write '#ifdef TEST_RUN' so that all subsequent code is compiled only when
;     building tests.
;   - Write '#ifdef MODULENAME_TESTS', where 'MODULENAME' is the unique name of
;     the module being tested. This value must be passed to the preprocessor
;     when building tests for this module.
;   - Connect the test.h file.
;   - Write functions for testing.
;   - Write RUN_TESTS (...), where the names of tests to be run should be listed
;     in brackets;
;   - Don't forget #endif for 'MODULENAME_TESTS' and 'TEST_RUN'.
;
;
; @date   October 2021
; @author Eph
;
-----------------------------------------------------------------------------**/



/* TEST_H */
#ifndef TEST_H
#define TEST_H

#ifdef TEST_RUN



/** @includes  --------------------------------------------------------------**/

#include <stdio.h>

/* Settings */
#define __CONSOLE_OUTPUT
#define __FILE_OUTPUT
#define __OUTPUT_A
//#define __OUTPUT_W



#ifdef __OUTPUT_W
#define __OUTPUT_A  // TODO: __OUTPUT_W has not fully implemented yet.
#endif /* __OUTPUT_W */

/* Additional protection against detecting two encodings at once. */
#ifdef __OUTPUT_A
#undef __OUTPUT_W
#elif defined __OUTPUT_W
#undef __OUTPUT_A
#endif /* __OUTPUT_W __OUTPUT_A */

/* Add 'L' at the beginning of the line for W-strings. */
#ifdef __OUTPUT_W
#define ___CAST_STRING(x) L ## x
#define __CAST_STRING(x) ___CAST_STRING(x)
#else
#define __CAST_STRING(x) x
#endif

/* Console output. */
#ifdef __CONSOLE_OUTPUT
#define __WRITE_IN_CONSOLE_A(format,...) do { printf(format,__VA_ARGS__); } while(0)
#define __WRITE_IN_CONSOLE_W(format,...) do { wprintf(format,__VA_ARGS__); } while(0)
#else
#define __WRITE_IN_CONSOLE_A ;
#define __WRITE_IN_CONSOLE_W ;
#endif

/* File output. */
#ifdef __FILE_OUTPUT

#define ___FRAME_WITH_DOUBLE_QUOTES(x) #x
#define __FRAME_WITH_DOUBLE_QUOTES(x) ___FRAME_WITH_DOUBLE_QUOTES(x)

#define __MM_CONCAT_( a, b )  a ## b
#define __MM_CONCAT( a, b )   __MM_CONCAT_( a, b )

FILE* open_test_report_file(void)
{
    FILE* __test_report_file_ptr = NULL;
    char __test_report_filename[] = 
        __FRAME_WITH_DOUBLE_QUOTES(__MM_CONCAT(TEST_MODULE, _TEST_REPORT.txt));
    if (0 != fopen_s(&__test_report_file_ptr, __test_report_filename, "a"))
    {
        return NULL;
    }
    return __test_report_file_ptr;
}

void close_test_report_file(FILE * file_ptr)
{
    fclose(file_ptr);
}

void delete_test_report_file(void)
{
    FILE* __test_report_file_ptr = NULL;
    char __test_report_filename[] =
        __FRAME_WITH_DOUBLE_QUOTES(__MM_CONCAT(TEST_MODULE, _TEST_REPORT.txt));
    if (0 == fopen_s(&__test_report_file_ptr, __test_report_filename, "w"))
    {
        fclose(__test_report_file_ptr);
    }
}
#define __WRITE_IN_FILE_A(format,...) do { FILE* __file = open_test_report_file(); fprintf_s(__file, format, __VA_ARGS__); close_test_report_file(__file); } while(0)
#define __WRITE_IN_FILE_W(format,...) do { FILE* __file = open_test_report_file(); fwprintf_s(__file, format, __VA_ARGS__); close_test_report_file(__file); } while(0)

#else
#define __WRITE_IN_FILE_A(format,...) ;
#define __WRITE_IN_FILE_W(format,...) ;
#endif

/* Configure the appropriate macros for outputs. */
#ifdef __OUTPUT_A
#define __WRITE_IN_FILE __WRITE_IN_FILE_A
#define __WRITE_IN_CONSOLE __WRITE_IN_CONSOLE_A
#elif defined __OUTPUT_W
#define __WRITE_IN_FILE __WRITE_IN_FILE_W
#define __WRITE_IN_CONSOLE __WRITE_IN_CONSOLE_W
#endif /* __OUTPUT_W __OUTPUT_A */

/* Set the output of information to the required streams (file/console),
   depending on the settings. */
#define __OUTPUT(...) do { __WRITE_IN_CONSOLE(__VA_ARGS__); __WRITE_IN_FILE(__VA_ARGS__); } while(0)
#define OUTPUT(...) __OUTPUT(__VA_ARGS__)


#define __PASS 0
#define __FAIL -1


// #define __PRINT_TEST_HEADER // TODO: Implement this macros.
#define __PRINT_TEST_FOOTER \
do \
{ \
    if(__test_result == __PASS) \
        { OUTPUT("PASSED (%d/%d)\n", __total_expecteds - __failed_expecteds, __total_expecteds); } \
    else \
        { OUTPUT("FAILED (%d/%d)\n", __total_expecteds - __failed_expecteds, __total_expecteds); } \
} while(0)


#define __PRINT_TESTS_SEPARATOR \
OUTPUT("-------------------------------------------------------------------------------\n")


#define TEST_BEGIN(__function_name) int __function_name(void) \
{ \
    OUTPUT("Executing test \'%s\'...\n", #__function_name); \
    int __test_result = __PASS; \
    int __total_expecteds = 0; \
    int __failed_expecteds = 0;

#define __TEST_END_ROUTINE \
__PRINT_TEST_FOOTER; \
return __test_result;

#define TEST_END \
    __TEST_END_ROUTINE \
}

#define __ENTRY_POINT \
int main(int argc, char* argv[], char* envp[]) \
{ \
    extern int(*__test_funcs[__MAX_TESTS_PER_MODULE])(void); \
    delete_test_report_file(); /* TODO: Delete only when __FILE_OUTPUT defined */ \
    int __global_result = __PASS; \
    for(int i = 0; i < __MAX_TESTS_PER_MODULE; i++) \
    {\
        if(__test_funcs[i] == NULL)\
            break; \
        __global_result += __test_funcs[i]();\
        __PRINT_TESTS_SEPARATOR; \
    }\
    getchar(); \
    return __global_result; \
}


#define __MAX_TESTS_PER_MODULE 100
#define __RUN_TESTS(...) int(*__test_funcs[__MAX_TESTS_PER_MODULE])(void) = { __VA_ARGS__ }; __ENTRY_POINT

#define RUN_TESTS __RUN_TESTS


/* Testing routine */

#define EXPECT(value, expected) \
do \
{ \
    __total_expecteds++; \
    if(value != expected) \
    { \
        __test_result = __FAIL; \
        __failed_expecteds++; \
    } \
} while(0)

#define EXPECT_NULL(value) \
EXPECT(value, NULL)

#define EXPECT_NOT_NULL(value) \
EXPECT(value == NULL, 0)

#define FORCE_FAIL_TEST \
do \
{ \
    OUTPUT("The test was forcibly failed.\n"); \
    __test_result = __FAIL; \
    __TEST_END_ROUTINE \
} while (0)


#endif /* __TEST_RUN */
#endif /* TEST_H */
