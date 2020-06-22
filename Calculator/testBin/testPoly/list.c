/**
 * @file list.c
 * @author Locietta
 * @brief list.h的实现 (GCC ONLY)
 * @details 带头双向链表以及简易迭代器
 * ----------------------------------------------------
 * - 加入了头结点
 * - 把(制造各种bug的)柔性数组成员模式换成了void *模式
 * @bug
 *
 * @version 0.2
 * @date 2020-06-07
 *
 *
 */

#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SWAP(a, b)                                                                                 \
    do {                                                                                           \
        typeof(a) _t = b;                                                                          \
        b = a;                                                                                     \
        a = _t;                                                                                    \
    } while (0)

/// 释放节点
#define freeNode(node)                                                                             \
    do {                                                                                           \
        this->destructor(node->data);                                                              \
        free(node);                                                                                \
    } while (0)

/// 链表函数实现
static iterator listBegin(List this) {
    return this->head->next;
}

static iterator listEnd(List this) {
    return this->head->prev;
}

const static void *listFront(List this) {
    return this->head->next->data;
}

const static void *listBack(List this) {
    return this->head->prev->data;
}

static int listEmpty(List this) {
    return !this->size;
}

static void listSetcmp(List this, int (*compare)(void *data1, void *data2)) {
    this->cmp = compare;
}

static void listSetDestructor(List this, void (*destructor)(void *_Block)) {
    this->destructor = destructor;
}

static void listPush_front(List this, void *newdata) {
    if (newdata != NULL) {
        Node *newNode = (Node *) malloc(sizeof(Node));
        newNode->data = malloc(this->elemSize);
        memmove(newNode->data, newdata, this->elemSize);

        newNode->next = this->head->next;
        newNode->prev = this->head;
        newNode->next->prev = newNode;
        newNode->prev->next = newNode;
        ++this->size;
    }
}

static void listPush_back(List this, void *newdata) {
    if (newdata != NULL) {
        Node *newNode = (Node *) malloc(sizeof(Node));
        newNode->data = malloc(this->elemSize);
        memmove(newNode->data, newdata, this->elemSize);

        newNode->next = this->head;
        newNode->prev = this->head->prev;
        newNode->next->prev = newNode;
        newNode->prev->next = newNode;
        ++this->size;
    }
}

static iterator listInsert(List this, iterator pos, void *newdata) {
    if (newdata == NULL) return NULL;

    Node *newNode = (Node *) malloc(sizeof(Node));
    newNode->data = malloc(this->elemSize);
    memmove(newNode->data, newdata, this->elemSize);

    newNode->next = pos;
    newNode->prev = pos->prev;
    newNode->prev->next = newNode;
    newNode->next->prev = newNode;
    ++this->size;

    return newNode;
}

static void listPop_front(List this) {
    Node *temp = this->head->next;
    this->head->next = temp->next;
    temp->next->prev = this->head;
    freeNode(temp);
    --this->size;
}

static void listPop_back(List this) {
    Node *temp = this->head->prev;
    this->head->prev = temp->prev;
    temp->prev->next = this->head;
    freeNode(temp);
    --this->size;
}

static iterator listErase(List this, iterator pos) {
    Node *ret = pos->next;
    pos->next->prev = pos->prev;
    pos->prev->next = pos->next;
    freeNode(pos);
    --this->size;
    return ret;
}

static void listRemove(List this, void *data) {
    if (this->cmp != NULL) {
        iterator cur = this->head->next;
        while (cur != this->head) {
            if (!this->cmp(cur->data, data)) {
                iterator temp = cur;
                cur = cur->next;
                temp->next->prev = temp->prev;
                temp->prev->next = temp->next;
                freeNode(temp);
                --this->size;
            } else {
                cur = cur->next;
            }
        }
    }
}

static void listRemove_if(List this, int (*key)(void *data)) {
    iterator cur = this->head->next;
    while (cur != this->head) {
        if (key(cur->data)) {
            iterator temp = cur;
            cur = cur->next;
            temp->next->prev = temp->prev;
            temp->prev->next = temp->next;
            freeNode(temp);
            --this->size;
        } else {
            cur = cur->next;
        }
    }
}

static void listFor_each(List this, iterator first, iterator last, void (*todo)(void *data)) {
    for (iterator it = first; it != last; it = it->next) {
        todo(it->data);
    }
}

static void listTraverse(List this, void (*todo)(void *data)) {
    listFor_each(this, this->head->next, this->head, todo);
}

static void listReverse(List this) {
    Node *current = this->head;
    for (int i = -1; i < this->size; ++i) {
        SWAP(current->next, current->prev);
        current = current->prev;
    }
}

// 链表快排实现

static iterator partition(List this, iterator low, iterator high) {
    iterator i = low;
    void *pivot = low->data;
    while (low != high) {
        while (low != high && this->cmp(high->data, pivot) >= 0) {
            high = high->prev;
        }
        while (low != high && this->cmp(pivot, low->data) >= 0) {
            low = low->next;
        }
        if (low != high) {
            SWAP(low->data, high->data);
        }
    }
    SWAP(low->data, i->data);
    return low;
}

static void qusort(List this, iterator low, iterator high) {
    if (low == high) {
        return;
    }
    iterator p = partition(this, low, high);
    if (low != p) {
        qusort(this, low, p->prev);
    }
    if (high != p) {
        qusort(this, p->next, high);
    }
}

static void listSort(List this) {
    if (this->cmp != NULL) {
        qusort(this, this->head->next, this->head->prev);
    }
}

static void listDestory(List this) {
    while (this->head->prev != this->head) {
        Node *temp = this->head->prev;
        this->head->prev = this->head->prev->prev;
        this->head->prev->next = this->head;
        freeNode(temp);
    }
    free(this->head);
    this->size = -1;
}

list listInit(size_t size) {
    list l = (list){.size = 0, .elemSize = size, .cmp = NULL, .destructor = free};
    l.head = (iterator) malloc(sizeof(Node));
    l.head->next = l.head->prev = l.head;

    l.empty = listEmpty;
    l.setcmp = listSetcmp;
    l.setDestructor = listSetDestructor;
    l.begin = listBegin;
    l.end = listEnd;
    l.front = listFront;
    l.back = listBack;
    l.push_front = listPush_front;
    l.push_back = listPush_back;
    l.insert = listInsert;
    l.pop_front = listPop_front;
    l.pop_back = listPop_back;
    l.erase = listErase;
    l.remove = listRemove;
    l.remove_if = listRemove_if;
    l.sort = listSort;
    l.reverse = listReverse;
    l.for_each = listFor_each;
    l.traverse = listTraverse;

    l.destory = listDestory;
    return l;
}