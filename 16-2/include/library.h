/**
 * File: library.h
 * @Locietta
 * -----------------------------------------------------
 * The library.h file defines the interface for a simple
 * library database.
 */

#ifndef LIBRARY_H
#define LIBRARY_H

#include "genlib.h"
#include "simpio.h"
#include "strlib.h"

#define MAXBOOKS 1000
#define MAXTITLE 31
#define MAXNAME 21
#define MAXSUB 21
#define MAXPUB 41

typedef struct {
    string title;
    string author[5];
    string cataNumber;
    string subHead[5];
    string publisher;
    string year;
    int isCirculating;
} * bookT;

typedef struct libraryDB_info *libraryDB;
struct libraryDB_info {
    bookT book[MAXBOOKS];
    int nBooks;
    /* Private function prototypes */
    void (*search)(libraryDB this, string key);
    void (*add)(libraryDB this);
    void (*destory)(libraryDB this);
};

/* Public functions prototypes */

libraryDB libInit(void);

#endif // (!defined LIBRARY_H)