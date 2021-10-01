#include <GLFW/glfw3.h>

#include "loop.h"
#include "window.h"
#include "../log.h"



/** @internal_prototypes -----------------------------------------------------*/
void _stubbed_loop_iteration_callback(void);



/** @functions ---------------------------------------------------------------*/
void start_loop(void(*loop_iteration_callback_ptr)(void))
{
    if (NULL == loop_iteration_callback_ptr)
    {
        // TODO: Check executable permissions, prologue bytes or canaries.
        loop_iteration_callback_ptr = _stubbed_loop_iteration_callback;
        LOG_WARNING(
            "No main loop callback specified. "
            "A plug is installed in its place.");
    }

    while (!glfwWindowShouldClose(get_glfw_window_ptr()))
    {
        /* Specify clear values for the color buffer */
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        /* Clear the 'GL_COLOR_BUFFER_BIT' buffer using the selected color */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Call a custom callback */
        loop_iteration_callback_ptr();

        /* Swap the front and back buffers */
        glfwSwapBuffers(get_glfw_window_ptr());

        /* Process all pending events */
        glfwPollEvents();
    }

    /* Destroy all windows, free allocated resources */
    glfwTerminate();
}


void _stubbed_loop_iteration_callback(void)
{

}
