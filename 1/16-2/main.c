#include <stdio.h>

#include "genlib.h"
#include "simpio.h"

#include "library.h"

void Test(libraryDB libdata);

int main(void) {
    libraryDB libdata = libInit();
    Test(libdata);
    libdata->destory(libdata);
    return 0;
}

void Test(libraryDB libdata) {
    for (int i = 0; i < 3; ++i) {
        libdata->add(libdata);
    }
    string buffer = NULL;
    libdata->search(libdata, buffer = GetLine());
    FreeBlock(buffer);
}