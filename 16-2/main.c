#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "genlib.h"
#include "simpio.h"
#include "strlib.h"

#define MAXBOOKS 1000
#define MAXTITLE 30

typedef struct {
    string title;
    string author[5];
    string cataNumber;
    string subHead[5];
    string publisher;
    string year;
    bool isCirculating;
} * bookT;

typedef struct libraryDB_info *libraryDB;
struct libraryDB_info {
    bookT book[MAXBOOKS];
    int nBooks;
    void (*search)(libraryDB libdata, string key);
    void (*read)(libraryDB libdata);
};

void libInit(libraryDB ptr);
void readBook(libraryDB libdata);
void searchBySubject(libraryDB libdata, string subject);

char *s_gets(char *st, int n);

int main(void) {

    return 0;
}

void libInit(libraryDB ptr) {
    ptr = New(libraryDB);
    ptr->nBooks = 0;
    ptr->search = searchBySubject;
    ptr->read = readBook;
}

void readBook(libraryDB libdata) {
    libdata->book[libdata->nBooks] = New(bookT);
    printf("Title:");
    s_gets(libdata->book[libdata->nBooks]->title, MAXTITLE);
    printf("Author(up to 5, one line with one name):");
    for (int i = 0; i < 5; ++i) {
        
    }
        
}

void searchBySubject(libraryDB libdata, string subject) {
    int booknum = libdata->nBooks;
    bool notfound = TRUE;
    for (int i = 0; i < booknum; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (libdata->book[i]->subHead[j] == subject) {
                notfound = FALSE;
                printf("Title:%s\tFirst author:%s\tLC catalog number:%s\n",
                       libdata->book[i]->title, libdata->book[i]->author[0], libdata->book[i]->cataNumber);
                break;
            }
        }
    }
    if (notfound) {
        puts("Sorry! There is no book with this subject in the database now.");
    }
}

char *s_gets(char *st, int n) {
    register char *ret_val;
    register char *find;

    ret_val = fgets(st, n, stdin);
    if (ret_val) {
        find = strchr(st, '\n'); //replace '\n' with '\0'
        if (find)
            *find = '\0';
        else
            while (getchar() != '\n') //eatline
                continue;
    }
    return ret_val;
}