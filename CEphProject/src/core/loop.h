#ifndef LOOP_H
#define LOOP_H



void start_loop(void(*loop_iteration_callback_ptr)(void));

float get_tick_count(void);
float get_frame_time(void);



#endif /* !LOOP */
