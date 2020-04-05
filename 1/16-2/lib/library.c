#include "library.h"

/* Private function prototypes */

static void addBook(libraryDB this);
static void searchBySubject(libraryDB this, string subject);
static void libDestory(libraryDB this);

/* Function implementation */

/* Public functions */

libraryDB libInit(void) {
    libraryDB newDB = New(libraryDB);
    newDB->nBooks = 0;
    newDB->search = searchBySubject;
    newDB->add = addBook;
    newDB->destory = libDestory;
    return newDB;
}

/* Private functions */

static void addBook(libraryDB this) {
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

static void searchBySubject(libraryDB this, string subject) {
    int bookNum = this->nBooks;
    bool notfound = TRUE;
    for (int i = 0; i < bookNum; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (this->book[i]->subHead[j]) { // shouldn't be NULL
                if (StringEqual(this->book[i]->subHead[j], subject)) {
                    notfound = FALSE;
                    printf("Title: %-20s\tFirst author: %-20s\tLibrary of Congress Catalog Number: %s\n",
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

static void libDestory(libraryDB this) {
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