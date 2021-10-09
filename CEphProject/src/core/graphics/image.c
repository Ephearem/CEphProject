#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include "image.h"
#include "../memory.h"
#include "../../log.h"



const stImage* load_image(const char* image_path)
{
    /* To flip loaded image on the y-axis */
    stbi_set_flip_vertically_on_load(1);

    stImage* image_ptr = m_malloc(sizeof(stImage));
    // TODO: Use another stbi function to load image bytes into memory using the
    //       'memory' module.

    image_ptr->data_ptr = stbi_load(
        image_path,
        &image_ptr->width,
        &image_ptr->height,
        &image_ptr->channels_count,
        0);

    if (NULL == image_ptr->data_ptr)
    {
        LOG_ERROR("Unable to load image [%s]. %s", image_path, image_ptr->data_ptr);
        m_free(image_ptr);
        return NULL;
    }
    return image_ptr;
}


void free_image(const stImage* image_ptr)
{
    if (NULL == image_ptr)
        return;

    if (image_ptr->data_ptr != NULL)
    {
        stbi_image_free(image_ptr->data_ptr);
    }
    m_free((void*)image_ptr);
}
