
#include "hash_map.h"
#include "types.h"
#include <stdlib.h>
#include <stdio.h>

/// @brief https://www.cse.yorku.ca/~oz/hash.html for info
/// @param str 
/// @return hash buffer str
static u64 djb2_hash(unsigned long prev, const unsigned char *str, size_t len);

static bool_t hm_needs_alloc(hm_t* m);
static int hm_realloc(hm_t* m);
static bool_t hm_slot_used(hm_t* m, int index);
static element_pair_t* hm_get_pair(hm_t* m, int index);
static element_pair_t hm_set_internal(hm_t* m, hm_key_t key, hm_value_t value);
static void hm_insert_element(hm_t* m, element_pair_t* pos, hm_key_t e_key, hm_value_t e_val);
static size_t hm_key_length(hm_key_t key);

element_pair_t hm_set(hm_t* m, hm_key_t key, hm_value_t value) {
    return hm_set_internal(m, key, value);
}

// assume for now capacity isn't changing a lot.
extern bool_t hm_get(hm_t* m, const hm_key_t key, hm_value_t* out) {
    element_pair_t* _pair;
    u64             index;
    size_t          keyLen;
    hm_value_t*     result = out;
    // *result                = HM_VALUE_NULL;

    keyLen = strlen(key);

    if (m->capacity <= 0)
        return FALSE;
    
    index = djb2_hash(0, key, keyLen) % m->items_capacity;
    _pair = hm_get_pair(m, index);
    if (_pair->key == NULL)
        return FALSE;

    if (strncmp(_pair->key, key, keyLen) == 0)
        *result = _pair->val;
    else {
        size_t index   = index + 1;
        size_t lookups = 0;

        while(lookups < m->items_capacity - 1) {
            ++lookups;

            _pair = hm_get_pair(m, (index++) % m->items_capacity);

            if (_pair->key == NULL) {
                // TODO:
                // end of this 1024 chunk?
                // if yes, rewrap it to find the element in this chunk
                // because maybe it lies in here and it has been wrapped around
                // instead of looking for the entire hashmap table to find the object.
                continue;
            }

            if (strncmp(_pair->key, key, keyLen) == 0) {
                *result = _pair->val;
                return TRUE;
            }
        }

        return FALSE;
    }

    return TRUE;
}

hm_t hm_new() {
    return CAST_TO(hm_t) {
        .value_size     = HM_VAL_SIZE,
        .capacity       = 0,
        
        .items          = 0,
        .items_capacity = 0,
        
        .used           = 0,
        .data           = NULL
    };
}

uintptr_t hm_addr(void* ptr) {
    return *((uintptr_t*) &ptr);
}

void hm_free(hm_t* m) {
    (void) m;
}

static void hm_insert_element(hm_t* m, element_pair_t* pos, hm_key_t e_key, hm_value_t e_val) {
    if (pos->key == NULL) {
        m->items++;
        m->used += sizeof(element_pair_t);
    }

    pos->key = e_key;
    pos->val = e_val;
}

element_pair_t hm_set_internal(hm_t* m, hm_key_t key, hm_value_t value) {
    if (hm_needs_alloc(m))
        hm_realloc(m);
        
    element_pair_t result = HM_EMPTY_ELEMENT_PAIR();
    u64 index;
    element_pair_t* _pair;
    size_t key_len = hm_key_length(key);

    if (m->capacity <= 0)
        return result;

    index = djb2_hash(0, (unsigned char*)(key), key_len) % m->items_capacity;
    _pair = hm_get_pair(m, index);

    if (_pair->key == NULL || strncmp(_pair->key, key, key_len) == 0)
        hm_insert_element(m, _pair, key, value);
    else
    {
        // look for a empty spot
        do {
            index = (index + 1) % m->capacity;
        } while((_pair = hm_get_pair(m, index))->key != NULL);

        hm_insert_element(m, _pair, key, value);
    }

    return *_pair;
}

static bool_t hm_needs_alloc(hm_t* m) {
    return m->used >= m->capacity;
}

static int hm_realloc(hm_t* m) {
    void* old           = m->data;
    size_t old_capacity = m->capacity;
    int status          = ALLOC_FLAG_OK;

    m->items_capacity += HM_LCAP_INCREMENT;
    m->capacity       += HM_LCAP_INCREMENT * sizeof(element_pair_t);
    m->data            = realloc(m->data, m->capacity);

    if (!m->data)
    {
        m->data     = old;
        m->capacity = old_capacity;
        status      = ALLOC_FLAG_ERR;
    } else {
        memset(
            (u8*)(m->data) + old_capacity, 
            0, 
            m->capacity - old_capacity
        );
    }

    return status;
}

static element_pair_t* hm_get_pair(hm_t* m, int index) {
    return ((element_pair_t*)(m->data)) + index;
}

static bool_t hm_slot_used(hm_t* m, int index) {
    return *((u8**)((element_pair_t*)(m->data) + index)) == NULL;
}

static u64 djb2_hash(unsigned long prev, const unsigned char *str, size_t len)
{
    unsigned long hash = 5381 + prev;
  

    for (size_t i = 0; i < len; ++i)
        hash = ((hash << 5) + hash) + str[i]; /* hash * 33 + c */

    return hash;
}


static size_t hm_key_length(hm_key_t key) {
    return strlen(key);
}
