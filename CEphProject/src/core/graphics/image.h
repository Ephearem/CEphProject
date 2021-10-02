#ifndef IMAGE_H
#define IMAGE_H



typedef struct
{
    char* data_ptr;
    int width;
    int height;
    int channels_count;
}stImage;



const stImage* load_image(const char* image_path);
void free_image(const stImage* image_ptr);



#endif /* !IMAGE_H */
