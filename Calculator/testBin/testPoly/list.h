/**
 * @file list.h
 * @author Locietta
 * @brief 突然发现链表有bug...推倒重来
 * @version 0.3
 * @date 2020-06-17 21:02
 *
 */

#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Public: interface */

// #define deref_iterator(type, it) (*((type *) it->data))

typedef struct Node Node, *iterator;
struct Node {
    Node *next, *prev;
    void *data;
};

typedef struct List_info list, *List;
struct List_info {
    iterator head;
    int size;
    size_t elemSize;
    int (*cmp)(void *data1, void *data2);
    void (*destructor)(void *_Block);

    /* Public Function Prototypes */
    int (*empty)(List this);
    void (*setcmp)(List this, int (*compare)(void *data1, void *data2));
    void (*setDestructor)(List this, void (*destructor)(void *_Block));

    iterator (*begin)(List this);
    iterator (*end)(List this);
    const void *(*front)(List this);
    const void *(*back)(List this);

    void (*push_front)(List this, void *newdata);
    void (*push_back)(List this, void *newdata);
    iterator (*insert)(List this, iterator pos, void *newdata);
    void (*pop_front)(List this);
    void (*pop_back)(List this);
    iterator (*erase)(List this, iterator pos);
    // iterator (*find)(List this, int (*key)(void *test_data, void *key_data), void *key_data);
    // List (*merge)(List this, List addOn);
    void (*remove)(List this, void *data);
    void (*remove_if)(List this, int (*key)(void *data));
    void (*sort)(List this);
    void (*reverse)(List this);
    void (*for_each)(List this, iterator first, iterator last, void (*todo)(void *data));
    void (*traverse)(List this, void (*todo)(void *data));

    void (*destory)(List this);
};

/* Construction Function */

list listInit(size_t size);

#define newList(type) listInit(sizeof(type))

#define deleteList(listA) (listA).destory(&listA)

#endif // (!defined LIST_H)