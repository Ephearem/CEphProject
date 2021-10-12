#include "list.h"
#include "../core/memory.h"



list* list_create(void)
{
    return (list*)m_calloc(1, sizeof(list));
}


void list_destroy(list* l)
{
    if (NULL == l)
        return;

    list_node* cur = l->nodes;

    while (cur)
    {
        list_node* tmp = cur->next;
        m_free(cur);
        cur = tmp;
    }
    m_free(l);
}


void list_push(list* l, void* data)
{
    if (NULL == l)
        return;
    if (NULL == l->nodes)
    {
        l->nodes = m_malloc(sizeof(list_node));
        l->nodes->data = data;
        l->nodes->next = NULL;
        return;
    }

    list_node* cur = l->nodes;
    while (cur->next)
        cur = cur->next;

    cur->next = (list_node*)m_malloc(sizeof(list_node));
    cur->next->data = data;
    cur->next->next = NULL;
}


int list_get_size(list* l)
{
    if (NULL == l)
        return 0;

    int result = 1;
    list_node* node = l->nodes;

    while (node)
    {
        result++;
        node = node->next;
    }
    return result;
}
