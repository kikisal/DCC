#pragma once

#include "types.h"
#include <stdarg.h>
#include <string.h>

#define S_FREE(ptr) do { \
    if (ptr)             \
        free(ptr);       \
} while(0)


#define ALLOC_FLAG_OK   0
#define ALLOC_FLAG_ERR -1

#define HM_LCAP_INCREMENT 1024 // 1k buffer linear step reallocation (in element units)
#define HM_VAL_SIZE sizeof(hm_value_t)

typedef uintptr_t hm_value_t;
typedef const u8*   hm_key_t;

#define HM_VALUE_NULL (hm_value_t) 0

// Elements are stored within these two pointers in memory.
typedef struct element_pair_st {
    hm_key_t   key;
    hm_value_t val;
} element_pair_t;

typedef struct hash_map_st {
    size_t value_size;

    // per item data.
    size_t items_capacity;
    size_t items;

    // byte counts
    size_t capacity;
    size_t used;

    void*  data;
} hm_t;

#define HM_EMPTY_HASHMAP() (hm_t) { \
    .value_size = 0,             \
    .capacity   = 0              \
}

#define HM_EMPTY_ELEMENT_PAIR() (element_pair_t) { \
    .key   = NULL,                                 \
    .val   = HM_VALUE_NULL                         \
}

/// @brief 
/// @param key_size 
/// @param value_size 
/// @return 
extern hm_t hm_new();
/// @brief 
/// @param m 
/// @param  
/// @return 
extern element_pair_t hm_set(hm_t* m, hm_key_t key, hm_value_t value);
/// @brief Returns pointer to item by "reference"
/// @param m 
/// @param key 
/// @param out 
extern bool_t hm_get(hm_t* m, const unsigned char* key, hm_value_t* out);

/// @brief converts pointer to a non-pointer type
/// @param ptr 
/// @return address of ptr of non-pointer type
extern uintptr_t hm_addr(void* ptr);

/// @brief Deallocates hashmap data and resets state of hash map for reuse.
/// @param m 
extern void hm_free(hm_t* m);


#define HM_PTR(ptr) (&ptr)
#define HMPTR(ptr)  (&ptr) // just another alias
#define HM_ADDR(o)     hm_addr(HM_PTR(o))
#define HM_ADDR_PTR(o) hm_addr(o)

#define HM_VAL_PTR(o, t) (*(t**)(&o));
