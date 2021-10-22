/**-----------------------------------------------------------------------------
; @file map.h
;
; @brief
;   // TODO: Balance.
;
; @date   October 2021
; @author Eph
;
-----------------------------------------------------------------------------**/


#ifndef MAP_H
#define MAP_H



#include <stddef.h>



typedef struct map map;



map* map_create(void);
void map_destroy(map* m);
void* map_search(map* m, size_t key);
void map_insert(map* m, size_t key, void* value);
void map_erase(map* m, size_t key);
void map_for_each_item(map* m, void(*func)(size_t, void*));
int map_get_size(map* m);

#endif /* !MAP_H */
