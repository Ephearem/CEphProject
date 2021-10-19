#include <vcruntime.h> /* NULL */

#include "map.h"
#include "../core/memory.h"



/** @types -------------------------------------------------------------------*/
typedef struct map_item map_item;

typedef struct map_item
{
    void* data;
    int key;
    map_item* left;
    map_item* right;
}map_item;


typedef struct map
{
    map_item* items;
    int size;
}map;



/** @internal_prototypes -----------------------------------------------------*/
static void _for_each_item_recursion(map_item* mi, void(*func)(int, void*));
static void _map_insert_item_no_recursion(map_item** node, int key, void* value);
static void _map_insert_item_recursion(map_item** node, int key, void* value);
static map_item* _map_search_item_no_recursion(map_item* i, int key);
static map_item* _map_search_item_recursion(map_item* i, int key);
static void _map_destroy_branch(map_item* item);
// static unsigned long hash_djb2(unsigned char* str)



/** @functions ---------------------------------------------------------------*/

map* map_create(void)
{
    map* m = m_malloc(sizeof(map));
    m->size = 0;
    m->items = NULL;
    //m->items = m_calloc(1, sizeof(map_item));
    return m;
}


void map_destroy(map* m)
{
    if (NULL == m)
        return;

    _map_destroy_branch(m->items);
    m_free(m);
}


void* map_search(map* m, int key)
{
    if (NULL == m)
        return NULL;
    if (NULL == m->items)
        return NULL;

    //return _map_search_item_recursion(m->items, key);
    return _map_search_item_no_recursion(m->items, key);
}


void map_insert(map* m, int key, void* value)
{
    _map_insert_item_no_recursion(&m->items, key, value);
    //_map_insert_item_recursion(&m->items, key, value);
    m->size++;
}


void map_erase(map* m, int key)
{
    map_item* i = m->items;
    map_item* prev = NULL;

    map_item* item_to_be_removed = NULL;

    while (i)
    {
        if (key > i->key)
        {
            prev = i;
            i = i->right;
        }
        else if (key < i->key)
        {
            prev = i;
            i = i->left;
        }
        else /* (key == i->key) */
        {
            if (NULL == i->left && NULL == i->right)
            {
                if (NULL == prev)
                {
                    m->items = i->right;
                }
                else if (prev->left == i)
                    prev->left = NULL;
                else if (prev->right == i)
                    prev->right = NULL;

                item_to_be_removed = i;
                break;
            }
            else if (i->left == NULL && i->right != NULL)
            {
                if (NULL == prev)
                {
                    m->items = i->right;
                }
                else if (prev->left == i)
                    prev->left = i->right;
                else
                    prev->right = i->right;
                item_to_be_removed = i;
                break;
            }
            else if (i->right == NULL && i->left != NULL)
            {
                if (prev->right == i)
                    prev->right = i->left;
                else
                    prev->left = i->left;
                item_to_be_removed = i;
                break;
            }
            else /* i->right != NULL && i->left != NULL*/
            {
                map_item** next = &(i->right);
                map_item* pv = *next; // TODO: NULL?
                while (1) // TODO: next->left
                {
                    if ((*next)->left == NULL)
                    {
                        pv->left = NULL;
                        break;
                    }
                    pv = *next;
                    next = &((*next)->left);
                }
                if (prev->left == i)
                    prev->left = (*next)->right;
                else /* prev->right == i */
                    prev->right = (*next)->right;
                

                if (prev->left == i)
                    prev->left = *next;
                else /* prev->right == i */
                    prev->right = *next;

                // i = next->right
                item_to_be_removed = i;
                break;
            }
        }
    }
    if (item_to_be_removed)
    {
        m->size--;
        m_free(item_to_be_removed); /* Remove */
    }
    return;
}


void map_for_each_item(map* m, void(*func)(int, void*))
{
    _for_each_item_recursion(m->items, func);
}


int map_get_size(map* m)
{
    if (NULL == m)
        return 0;
    return m->size;
}


static void _for_each_item_recursion(map_item* mi, void(*func)(int, void*))
{
    if (NULL == mi)
        return;
    _for_each_item_recursion(mi->left, func);
    _for_each_item_recursion(mi->right, func);
    func(mi->key, mi->data);
}

static void _map_insert_item_no_recursion(map_item** node, int key, void* value)
{
    while (*node)
    {
        if (key > (*node)->key)
            node = &((*node)->right);
        else if (key < (*node)->key)
            node = &((*node)->left);
    }
    *node = m_malloc(sizeof(map_item));
    (*node)->data = value;
    (*node)->key = key;
    (*node)->left = NULL;
    (*node)->right = NULL;
}


static void _map_insert_item_recursion(map_item** node, int key, void* value)
{
    if (NULL == *node)
    {
        *node = m_malloc(sizeof(map_item));
        (*node)->key = key;
        (*node)->data = value;
        (*node)->left = NULL;
        (*node)->right = NULL;
    }
    else if (key > (*node)->key)
    {
        _map_insert_item_recursion(&((*node)->right), key, value);
    }
    else if (key < (*node)->key)
    {
        _map_insert_item_recursion(&((*node)->left), key, value);
    }
    else /* Replace */
    {
        (*node)->data = value;
    }
    return;
}


static map_item* _map_search_item_no_recursion(map_item* i, int key)
{
    if (NULL == i)
        return NULL;

    while (i)
    {
        if (key > i->key)
        {
            i = i->right;
            continue;
        }
        else if (key < i->key)
        {
            i = i->left;
            continue;
        }
        else /* (key == i->key) */
            return i->data;
    }
    return NULL;
}


static map_item* _map_search_item_recursion(map_item* i, int key)
{
    if (NULL == i)
        return NULL;

    if (key == i->key)
    {
        return i->data;
    }
    else if (key < i->key)
        return _map_search_item_recursion(i->left, key);
    else
        return _map_search_item_recursion(i->right, key);
}


static void _map_destroy_branch(map_item* i)
{
    if (NULL == i)
        return;
    _map_destroy_branch(i->left);
    _map_destroy_branch(i->right);
    m_free(i);
}


// static unsigned long hash_djb2(unsigned char* str)
// {
//     unsigned long hash = 5381;
//     int c;
// 
//     while (c = *str++)
//         hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
// 
//     return hash;
// }
