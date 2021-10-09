#include "../core/memory.h"

#include "list.h"



//list_node* list_create(void)
//{
//    return (list_node*)calloc(1, sizeof(list_node));
//}


void list_push(list_node* root, void* data)
{
    list_node* cur = root;

    while (cur->next)
        cur = cur->next;

    cur->next = (list_node*)m_malloc(sizeof(list_node));
    cur->next->data = data;
    cur->next->next = NULL;
}


void list_cleanup(list_node* root)
{
    list_node* cur = root;

    while (cur)
    {
        list_node* tmp = cur->next;
        m_free(cur);
        cur = tmp;
    }
}


int list_get_size(list_node* root)
{
    if (NULL == root)
        return 0;

    int result = 1;
    list_node* tmp = root;

    while (tmp->next != NULL)
    {
        result++;
        tmp = tmp->next;
    }
    return result;
}
