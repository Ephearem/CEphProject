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


list_node* list_push(list* l, void* data)
{
    if (NULL == l)
        return NULL;
    if (NULL == l->nodes)
    {
        l->nodes = m_malloc(sizeof(list_node));
        l->nodes->data = data;
        l->nodes->next = NULL;
        return l->nodes;
    }

    list_node* cur = l->nodes;
    while (cur->next)
        cur = cur->next;

    cur->next = (list_node*)m_malloc(sizeof(list_node));
    cur->next->data = data;
    cur->next->next = NULL;
    return cur->next;
}


void list_erase(list* l, list_node* node)
{
    if (NULL == l)
        return;
    if (NULL == l->nodes)
        return;

    if (node == l->nodes)
    {
        l->nodes = l->nodes->next;
        m_free(node);
        return;
    }

    list_node* prev_node = l->nodes;
    list_node* cur_node = l->nodes->next;
    while (cur_node)
    {
        if (cur_node == node)
        {
            prev_node->next = cur_node->next;
            m_free(cur_node);
            return;
        }
        prev_node = cur_node;
        cur_node = cur_node->next;
    }
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


int list_is_empty(list* l)
{
    if (NULL == l)
        return 1;
    if (NULL == l->nodes)
        return 1;

    return 0;
}
