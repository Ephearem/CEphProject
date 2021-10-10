#include <stdlib.h>

#include "memory.h"
#include "../log.h"



void* m_malloc(size_t size)
{
    void* result = malloc(size);
    //LOG_MSG("m_malloc: %d [%p]", size, result);
    if (NULL == result)
    {
        LOG_ERROR("Failed to allocate %zu bytes.", size);
    }
    return result;
}


void* m_calloc(size_t count, size_t size)
{
    void* result = calloc(count, size);
    //LOG_MSG("m_calloc: %d [%p]", size, result);
    if (NULL == result)
    {
        LOG_ERROR("Failed to allocate %zu bytes.", count * size);
    }
    return result;
}

void* m_realloc(void* ptr, size_t new_size)
{
    void* result = NULL;
    //LOG_MSG("m_realloc [%p]", ptr);
    result = realloc(ptr, new_size);
    if (NULL == ptr)
    {
        LOG_ERROR("Failed to reallocate %zu bytes.", new_size);
    }
    return result;
}


void m_free(void* ptr)
{
    //LOG_MSG("m_free [%p]", ptr);
    free(ptr);
}
