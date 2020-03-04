#include <stdio.h>

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
    void (*search)(libraryDB this, string key);
    void (*add)(libraryDB this);
    void (*destory)(libraryDB this);
};

void Test(libraryDB libdata);

libraryDB libInit(void);
void addBook(libraryDB this);
void searchBySubject(libraryDB this, string subject);
void libDestory(libraryDB this);

int main(void) {
    libraryDB libdata = libInit();
    Test(libdata);
    libdata->destory(libdata);
    return 0;
}

libraryDB libInit(void) {
    libraryDB newDB = New(libraryDB);
    newDB->nBooks = 0;
    newDB->search = searchBySubject;
    newDB->add = addBook;
    newDB->destory = libDestory;
    return newDB;
}

void addBook(libraryDB this) {
    this->book[this->nBooks] = New(bookT);
    printf("Title:\n");
    this->book[this->nBooks]->title = GetLine();
    printf("Author(up to 5, one line with one name):\n");
    for (int i = 0; i < 5; ++i) {
        this->book[this->nBooks]->author[i] = GetLine();
        if (*this->book[this->nBooks]->author[i] == '\0') {
            break;
        }
    }
    printf("The Library of Congress Catalog Number:\n");
    this->book[this->nBooks]->cataNumber = GetLine();
    printf("Subject(up to 5, one line with one subject):\n");
    for (int i = 0; i < 5; ++i) {
        this->book[this->nBooks]->subHead[i] = GetLine();
        if (*this->book[this->nBooks]->subHead[i] == '\0') {
            break;
        }
    }
    printf("Publisher:\n");
    this->book[this->nBooks]->publisher = GetLine();
    printf("Year:\n");
    this->book[this->nBooks]->year = GetLine();
    printf("Circulating situation(1 for yes, 0 for no):\n");
    scanf("%d", &this->book[this->nBooks]->isCirculating);
    while (getchar() != '\n') //eatline
        continue;
    this->nBooks++;
}

void searchBySubject(libraryDB this, string subject) {
    int bookNum = this->nBooks;
    bool notfound = TRUE;
    for (int i = 0; i < bookNum; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (this->book[i]->subHead[j]) { // shouldn't be NULL
                if (StringEqual(this->book[i]->subHead[j], subject)) {
                    notfound = FALSE;
                    printf("Title: %s\t\tFirst author: %s\t\tLibrary of Congress Catalog Number: %s\n",
                           this->book[i]->title, this->book[i]->author[0],
                           this->book[i]->cataNumber);
                    break;
                }
            }
        }
    }
    if (notfound) {
        puts("Sorry! There is no book with this subject in the database now.");
    }
}

void libDestory(libraryDB this) {
    int bookNum = this->nBooks;
    for (int i = 0; i < bookNum; ++i) {
        FreeBlock(this->book[i]->title);
        for (int j = 0; j < 5; ++j) {
            FreeBlock(this->book[i]->author[j]);
        }
        FreeBlock(this->book[i]->cataNumber);
        FreeBlock(this->book[i]->publisher);
        FreeBlock(this->book[i]->year);
        for (int j = 0; j < 5; ++j) {
            FreeBlock(this->book[i]->subHead[i]);
        }
        FreeBlock(this->book[i]);
    }
    FreeBlock(this);
}

void Test(libraryDB libdata) {
    for (int i = 0; i < 3; ++i) {
        libdata->add(libdata);
    }
    string buffer = NULL;
    libdata->search(libdata, buffer = GetLine());
    FreeBlock(buffer);
}