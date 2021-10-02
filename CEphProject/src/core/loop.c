#include <GLFW/glfw3.h>

#include "loop.h"
#include "window.h"
#include "../log.h"



/** @static_data -------------------------------------------------------------*/
static float _tick_count = 0.0f;
static float _frame_time = 0.0f;



/** @internal_prototypes -----------------------------------------------------*/
static void _stubbed_loop_iteration_callback(void);
static float _calc_fps(float period);


/** @functions ---------------------------------------------------------------*/
void start_loop(void(*loop_iteration_callback_ptr)(void))
{
    extern float _tick_count;
    extern float _frame_time;

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
        /* Sync */
        static float prev_tick_count = 0.0f;
        _tick_count = (float)glfwGetTime();
        _frame_time = _tick_count - prev_tick_count;
        prev_tick_count = _tick_count;
        _calc_fps(1.0f);

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


float get_tick_count(void)
{
    extern float _tick_count;
    return _tick_count;
}


float get_frame_time(void)
{
    extern float _frame_time;
    return _frame_time;
}


static void _stubbed_loop_iteration_callback(void)
{

}


static float _calc_fps(float period)
{
    extern float _frame_time;

    static float result = 0.0f;

    static float time_since_last_update = 0.0;
    static unsigned int ticks_since_last_update = 0;
    time_since_last_update += _frame_time;
    ticks_since_last_update++;

    if (time_since_last_update >= period)
    {
        float extra_time = time_since_last_update - period;
        result = (ticks_since_last_update + extra_time) /
            time_since_last_update;
        // glfwSetWindowTitle
        ticks_since_last_update = 0;
        time_since_last_update = 0.0;
        LOG_MSG("%f", result);
    }
    return result;
}
