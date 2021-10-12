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



list_node* list_create(void);
void list_push(list_node* root, void* data);
void list_cleanup(list_node* root);
int list_get_size(list_node* root);



#endif /* !LIST_H */
