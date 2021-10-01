#ifndef WINDOW_H
#define WINDOW_H



int init_window(const char* title, int width, int height, int is_fullscreen,
    int swap_interval);
void* get_glfw_window_ptr(void);
int get_window_width(void);
int get_window_height(void);



#endif /* !WINDOW_H */
