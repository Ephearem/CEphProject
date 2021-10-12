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



typedef struct map map;


map* map_create(void);
void map_destroy(map* m);
void* map_search(map* m, int key);
void map_insert(map* m, int key, void* value);
void map_erase(map* m, int key);



#endif /* !MAP_H */
