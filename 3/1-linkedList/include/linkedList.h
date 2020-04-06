/*
 * File: linkedList.h
 * -------------
 * This file provides an interface to a simple linkedList
 * abstraction.
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stddef.h>

typedef struct Node Node;
struct Node {
    Node *next, *prev;
    size_t size;
    char data[]; // C99
};

typedef struct List_info *List;
struct List_info {
    Node *head;
    int len;
    /* Other Public Function Prototypes */
    int (*isempty)(List this);
    Node *(*begin)(List this);
    Node *(*end)(List this);
    int (*insert)(List this, Node *position, void *newdata, size_t size);
    int (*headInsert)(List this, void *newdata, size_t size);
    int (*tailInsert)(List this, void *newdata, size_t size);
    int (*delete)(List this, Node *position);
    Node *(*find)(List this, int (*key)(void *test_data, void *key_data), void *key_data);
    int (*modify)(List this, Node *position, void *newdata, size_t size);
    void (*sort)(List this, int (*isBigger)(void *data1, void *data2), int increase);
    List (*merge)(List this, List addOn);
    List (*inverse)(List this);
    void (*foreach)(List this, void (*todo)(Node *node));
    
    void (*destory)(List this);
};

/* Construction Function Prototype */

List listInit(void);



#endif // (!defined LINKEDLIST_H)
