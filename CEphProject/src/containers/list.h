#ifndef LIST_H
#define LIST_H



#define LIST_PUSH(ROOT, DATA) \
do { \
    if(NULL == ROOT) {ROOT = m_calloc(1, sizeof(list_node)); (void*)(ROOT)->data = DATA; } \
    else list_push(ROOT, DATA); \
} while(0)



typedef struct list_node
{
    void* data;
    struct list_node* next;
}list_node;


typedef struct list
{
    list_node* nodes;
}list;



list* list_create(void);
void list_destroy(list* l);
list_node* list_push(list* l, void* data);
void list_erase(list* l, list_node* node);
int list_get_size(list* l);
int list_is_empty(list* l);



#endif /* !LIST_H */
