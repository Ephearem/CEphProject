#ifndef MEMORY_H
#define MEMORY_H



#include <vcruntime.h> /* size_t */



void* m_malloc(size_t size);
void* m_calloc(size_t count, size_t size);
void* m_realloc(void* ptr, size_t new_size);
void m_free(void* ptr);



#endif /* !MEMORY_H */
