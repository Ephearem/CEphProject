#ifndef WINDOW_H
#define WINDOW_H



int window_init(const char* title, int width, int height, int is_fullscreen,
    int swap_interval);
void* window_get_glfw_window_ptr(void);
int window_get_width(void);
int window_get_height(void);



#endif /* !WINDOW_H */
