// css generator

#include <stdio.h>
#include "html_parser.h"
#include "css_generator.h"
#include "hash_map.h"

#include <stdlib.h>

typedef struct { 
    int p1;
    int p2;
    int p3;
    int p4;
    int p5;
}  Monster;

int main() {

    // ----------- STORING INTEGER VALUE 123 IN THE HASH MAP -----------

    hm_t map = hm_new();

    hm_set(HM_PTR(map), "hello", 123);
    
    hm_value_t val = 0; // set to default value prior the call.
    bool_t found = hm_get(HM_PTR(map), "hello", &val);
    

    printf("STORING INTEGER VALUE: \n");
    printf("Value(%lld), found(%d)\n", val, found);
    printf("\n");

    // ----------- STORING A BIGGER OBJECT IN THE STACK BY ITS POINTER (Not Reccomended) ----------------
 
    Monster m; // allocated in the stack.
    m.p1 = 213;
    m.p5 = 1000;
    m.p2 = 12322;

    hm_set(HM_PTR(map), "monster_stack", HM_ADDR(m));

    val = 0;
    found = hm_get(HM_PTR(map), "monster_stack", &val);

    printf("STORING POINTER TO VALUE IN THE STACK: \n");
    printf("Value(%lld), found(%d), %p\n", val, found, &m);

    if (found) {
        Monster* p = HM_VAL_PTR(val, Monster);
        printf("p1(%d), p5(%d), p2(%d)\n", p->p1, p->p5, p->p2);
    }
    printf("\n");

    // ----------- STORING A BIGGER OBJECT IN THE STACK BY ITS POINTER (Reccomended) ----------------

    Monster* m2 = calloc(1, sizeof(Monster));
    m2->p1 = 50;
    m2->p2 = 51;
    m2->p3 = 52;

    hm_set(HM_PTR(map), "monster_heap", HM_ADDR_PTR(m2)); // use HM_ADDR_PTR for pointer types (Monster * m2)
    val = 0;
    found = hm_get(HM_PTR(map), "monster_heap", &val);

    printf("STORING POINTER TO VALUE IN THE HEAP: \n");
    printf("Value(%lld), found(%d), %p\n", val, found, m2);

    if (found) {
        Monster* p = HM_VAL_PTR(val, Monster);
        printf("p1(%d), p2(%d), p3(%d)\n", p->p1, p->p2, p->p3);
    }
    printf("\n");

    printf("---------------------\n");
    printf("Hash map state: Items(%lld), Capacity(%lld) \n", map.items, map.items_capacity);
    printf("\n");

    hm_free(HM_PTR(map));

    return 0;
}