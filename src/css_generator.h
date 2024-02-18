#pragma once

#include "html_types.h"
#include "types.h"

typedef struct CSS_doc_st {
    
} CSS_doc;

/// @brief generates a css document result from the HTMLNode looking at all of the node's attributes attr
/// @param node 
/// @param result
/// @param attr
extern void CSS_compile(HTMLNode node, CSS_doc* result, const char* attr);
extern void CSS_free_doc(CSS_doc* doc);
extern void CSS_file_write(CSS_doc doc, const char* filePath);

