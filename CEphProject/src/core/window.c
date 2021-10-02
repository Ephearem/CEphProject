#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include "../log.h"



/** @static_data -------------------------------------------------------------*/
static GLFWwindow* _window_ptr = NULL;
static int _width;
static int _height;



/** @internal_prototypes -----------------------------------------------------*/
static void _glfw_error_callback(int code, const char* message);



/** @functions ---------------------------------------------------------------*/
int init_window(const char* title, int width, int height, int is_fullscreen,
    int swap_interval)
{
    extern GLFWwindow* _window_ptr;
    extern int _width;
    extern int _height;

    GLFWmonitor* monitor_ptr = NULL;

    /* Initialize and configure GLFW */
    glfwInit();
    
    glfwSetErrorCallback(_glfw_error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (is_fullscreen)
    {
        monitor_ptr = glfwGetPrimaryMonitor();
    }

    /* Get the available video modes for the rimary monitor */
    const GLFWvidmode* vidmode_ptr = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (width > vidmode_ptr->width || height > vidmode_ptr->height)
    {
        LOG_ERROR("Invalid window size %dx%d.", width, height);
        glfwTerminate();
        return -1;
    }

    _window_ptr = glfwCreateWindow(width, height, title, monitor_ptr, NULL);

    /* Create the window */
    if (NULL == _window_ptr)
    {
        return -1;
    }
    glfwMakeContextCurrent(_window_ptr);

    /* Set the number of screen updates to wait from the time */
    /* 'glfwSwapBuffers' was called before swapping the buffers and returning */
    /* (i.e. vsync) */
    glfwSwapInterval(swap_interval);

    /* Initialize GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }
    glViewport(0, 0, width, height);

    /* Enable GL_BLEND to support transparent textures */
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _width = width;
    _height = height;

    return 0;
}

void* get_glfw_window_ptr(void)
{
    return _window_ptr;
}


int get_window_width(void)
{
    return _width;
}


int get_window_height(void)
{
    return _height;
}


static void _glfw_error_callback(int code, const char* message)
{
    LOG_ERROR("%d. %s.", code, message);
    glfwTerminate();
}



/** @tests-------------------------------------------------------------------**/
//#define TEST_RUN
//#define WINDOW_TEST
//#define TEST_MODULE WINDOW

#ifdef TEST_RUN
#ifdef WINDOW_TEST

#include "../test.h"


/**-----------------------------------------------------------------------------
; @unit_test
;
; @brief
;   // TODO: Write brief.
;
-----------------------------------------------------------------------------**/
TEST_BEGIN(test_invalid_window_size)
{
    EXPECT_NOT_ZERO(init_window("test", 0, 100, 0, 0));
    EXPECT_NULL(_window_ptr);
    EXPECT(_width, 0);
    EXPECT(_height, 0);
    _width = 0;
    _height = 0;
    glfwTerminate();

    EXPECT_NOT_ZERO(init_window("test", 100, 0, 0, 0));
    EXPECT_NULL(_window_ptr);
    EXPECT(_width, 0);
    EXPECT(_height, 0);
    _width = 0;
    _height = 0;
    glfwTerminate();

    EXPECT_NOT_ZERO(init_window("test", -1, 1, 0, 0));
    EXPECT_NULL(_window_ptr);
    EXPECT(_width, 0);
    EXPECT(_height, 0);
    _width = 0;
    _height = 0;
    glfwTerminate();

    EXPECT_NOT_ZERO(init_window("test", 1, -1, 0, 0));
    EXPECT_NULL(_window_ptr);
    EXPECT(_width, 0);
    EXPECT(_height, 0);
    _width = 0;
    _height = 0;
    glfwTerminate();

    EXPECT_NOT_ZERO(init_window("test", 65535, 65535, 0, 0));
    EXPECT_NULL(_window_ptr);
    EXPECT(_width, 0);
    EXPECT(_height, 0);
    _width = 0;
    _height = 0;
    glfwTerminate();

    EXPECT_ZERO(init_window("test", 1, 1, 0, 0));
    EXPECT_NOT_NULL(_window_ptr);
    EXPECT(_width, 1);
    EXPECT(_height, 1);
    _width = 0;
    _height = 0;
    glfwTerminate();
    TEST_END
}


RUN_TESTS
(
    test_invalid_window_size
)


#endif /* WINDOW_TEST */
#endif /* TEST_RUN */
