/**
 * File: linkedList.c
 * @Locietta
 * -----------------------------------------------------
 * This file implements the linkedList.h interface by 
 * a double-link circular list without head.
 */

#define NDEBUG

#include "linkedList.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct Node {
//     Node *next, *prev;
//     char data[]; // C99
// };

/* Private functions prototypes */

// static void freeNode(Node *Node);
static int NodeSwap(Node *node1, Node *node2);
static void nodeInverse(Node *node);
// static void nodePrint(Node *node);
/* Public functions prototypes */

static int listIsempty(List this);
static Node *listBegin(List this);
static Node *listEnd(List this);

static int listInsert(List this, Node *position, void *newdata, size_t size);
static int listHeadInsert(List this, void *newdata, size_t size);
static int listTailInsert(List this, void *newdata, size_t size);
static int listDelete(List this, Node *position);
static Node *listFind(List this, int (*key)(void *test_data, void *key_data), void *key_data);
static int listModify(List this, Node *position, void *newdata, size_t size);

static void listSort(List this, int (*isBigger)(void *data1, void *data2), int increase);
static List listMerge(List this, List addOn);
static List listInverse(List this);
static void listFor_each_safe(List this, void (*todo)(Node *));
// static void listDisplay(List this);

static void listDestory(List this);
/* Function implementations */

/* Public functions */

List listInit(void) {
    List newList = (List) malloc(sizeof(struct List_info));
    newList->len = 0;
    newList->head = (Node *) malloc(sizeof(Node));
    newList->head->prev = newList->head->next = newList->head;
    newList->head->size = 0;

    newList->isempty = listIsempty;
    newList->begin = listBegin;
    newList->end = listEnd;
    newList->insert = listInsert;
    newList->headInsert = listHeadInsert;
    newList->tailInsert = listTailInsert;
    newList->delete = listDelete;
    newList->find = listFind;
    newList->modify = listModify;
    newList->sort = listSort;
    newList->merge = listMerge;
    newList->inverse = listInverse;
    newList->foreach = listFor_each_safe;
    newList->destory = listDestory;
    return newList;
}

static int listIsempty(List this) {
    return (!this->len);
}

static Node *listBegin(List this) {
    return this->head;
}

static Node *listEnd(List this) {
    return this->head->prev;
}

static int listInsert(List this, Node *position, void *newdata, size_t size) {
    if (position == this->head) {
        if (this->isempty(this)) {
            this->head = (Node *) realloc(this->head, sizeof(Node) + size);
            this->head->prev = this->head->next = this->head;
            this->head->size = size;
            memmove(this->head->data, newdata, size);
            this->len++;
        } else {
            Node *newNode = (Node *) malloc(sizeof(Node) + size);
            newNode->prev = this->head->prev;
            this->head->prev = newNode;
            newNode->next = this->head;
            newNode->prev->next = newNode;

            memmove(newNode->data, newdata, size);
            newNode->size = size;
            this->head = newNode;
            this->len++;
        }
    } else {
        int i = 1;
        for (Node *current = this->head->next; i < this->len; ++i) {
            if (current == position) {
                Node *newNode = (Node *) malloc(sizeof(Node) + size);
                newNode->prev = current->prev;
                current->prev = newNode;
                newNode->next = current;
                newNode->prev->next = newNode;

                memmove(newNode->data, newdata, size);
                newNode->size = size;
                this->len++;
                break;
            }
        }

        if (i >= this->len) {
            puts("Wrong Position for Insertion");
            return 1;
        }
    }
    return 0;
}

static int listHeadInsert(List this, void *newdata, size_t size) {
    return listInsert(this, this->head, newdata, size);
}

static int listTailInsert(List this, void *newdata, size_t size) {
    int ret = listInsert(this, this->head, newdata, size);
    if (!ret) {
        this->head = this->head->next;
    }
    return ret;
}

static int listDelete(List this, Node *position) {
    if (this->isempty(this)) {
        puts("Deletion from an empty list");
        return 1;
    } else if (position == this->head) {
        Node *current = this->head;
        this->head = this->head->next;
        this->head->prev = this->head->prev->prev;
        this->head->prev->next = this->head;
        free(current); // 数据域和指针域一起释放，没有内存泄漏
        this->len--;
    } else {
        int i = 1;
        for (Node *current = this->head->next; i < this->len; ++i) {
            if (current == position) {
                current = current->next;
                current->prev = current->prev->prev;
                current->prev->next = current;
                free(position);
                break;
            }
            current = current->next;
        }

        if (i >= this->len) {
            puts("Wrong Position for Deletion");
            return 1;
        } else {
            this->len--;
        }
    }
    return 0;
}

// 这看上去很蠢，我想要重载(哭
static Node *listFind(List this, int (*key)(void *test_data, void *key_data), void *key_data) {
    assert(key != NULL);

    if (this->isempty(this)) {
        // puts("Find in an empty List");
        return NULL;
    } else {
        Node *current = this->head;
        for (int i = 0; i < this->len; ++i) {
            if (key(current->data, key_data)) {
                return current;
            }
            current = current->next;
        }
    }
    return NULL;
}

static int listModify(List this, Node *position, void *newdata, size_t size) {
    if (this->isempty(this)) {
        puts("Modify in an empty list");
        return 1;
    }

    int i = 0;
    for (Node *current = this->head; i < this->len; ++i) {
        if (current == position) {
            Node nodecpy = *current;
            Node *newcurrent = (Node *) realloc(current, sizeof(Node) + size); // realloc is a bad function...
            if (newcurrent == NULL) {
                puts("Storage allocation error");
                return 1;
            } else if (newcurrent != current) {
                newcurrent->next = nodecpy.next;
                newcurrent->prev = nodecpy.prev;
                newcurrent->next->prev = newcurrent->prev->next = newcurrent;
            }
            newcurrent->size = size;
            memmove(newcurrent->data, newdata, size);
            break;
        }
        current = current->next;
    }

    if (i >= this->len) {
        puts("Wrong Position for Modification");
        return 1;
    }

    return 0;
}

static void listSort(List this, int (*isBigger)(void *data1, void *data2), int increase) {
    Node *current = this->head;
    for (int i = 1; i <= this->len - 1; ++i) {
        Node *index = current->next, *extreme = current;
        for (int j = i + 1; j <= this->len; ++j) {
            if (!(increase ^ isBigger(extreme->data, index->data))) {
                // 同或 (increase && isBigger(extreme->data, index->data)) ||
                //     (!increase && !isBigger(extreme->data, index->data))
                extreme = index;
            }
            index = index->next;
        }
        NodeSwap(extreme, current);
        current = current->next;
    }
}

static List listMerge(List this, List addOn) {
    if (!addOn->isempty(addOn)) {
        Node *current = addOn->head;
        for (;; current = current->next) {
            this->tailInsert(this, current->data, current->size);
            if (current == addOn->end(addOn)) {
                break;
            }
        }
    }
    return this;
}

static List listInverse(List this) {
    this->head = this->head->prev;
    this->foreach(this, nodeInverse);
    return this;
}

static void listFor_each_safe(List this, void (*todo)(Node *)) {
    int i = 0;
    for (Node *cur = this->head; i < this->len; cur = cur->next) {
        todo(cur);
        i++;
    }
}

// static void listDisplay(List this) {

// }

static void listDestory(List this) {
    while (!this->isempty(this)) {
        listDelete(this, this->head);
    }
    free(this);
}

/* Private Functions */

static void nodeInverse(Node *node) {
    Node *temp = node->prev;
    node->prev = node->next;
    node->next = temp;
}

static int NodeSwap(Node *node1, Node *node2) {
    if (node1 != node2) {
        Node nodecpy1 = *node1, nodecpy2 = *node2;
        void *buffer = malloc(nodecpy1.size);

        memmove(buffer, node1->data, nodecpy1.size);
        Node *newnode1 = (Node *) realloc(node1, sizeof(Node) + nodecpy2.size); // i hate realloc
        if (newnode1 != node1) {
            node1->next = nodecpy1.next;
            node1->prev = nodecpy1.prev;
            node1->next->prev = node1->prev->next = node1;
        }
        node1->size = nodecpy2.size;
        memmove(node1->data, node2->data, nodecpy2.size);

        Node *newnode2 = (Node *) realloc(node2, sizeof(Node) + nodecpy1.size);
        if (newnode2 != node2) {
            node2->next = nodecpy2.next;
            node2->prev = nodecpy2.prev;
            node2->next->prev = node2->prev->next = node2;
        }
        node2->size = nodecpy1.size;
        memmove(node2->data, buffer, nodecpy1.size);

        free(buffer);
    }
    return 0;
}

// static void nodePrint(Node *node) {
//     printf("%d ", *((int *) node->data));
// }