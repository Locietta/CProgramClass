#include <stdio.h>
#include <ctype.h>
#include <string.h>

char table[10][3] = {{-1, -1, -1},    {-1, -1, -1},    {'A', 'B', 'C'},
                     {'D', 'E', 'F'}, {'G', 'H', 'I'}, {'J', 'K', 'L'},
                     {'M', 'N', 'O'}, {'P', 'R', 'S'}, {'T', 'U', 'V'},
                     {'W', 'X', 'Y'}};

void ListMnemonics(const char *str);

int isDigitString(const char *str);

int main(void) {
    char teleNumber[50];
    printf("This program displays mnemonics for a telephone number.\n");
    printf("Your telephone number:");
    while (EOF != scanf("%s", teleNumber)) {
        if (isDigitString(teleNumber)) {
            ListMnemonics(teleNumber);
        } else {
            puts("Not a number string!");
        }
        printf("\nYour telephone number:");
    }
}

int isDigitString(const char *str) {
    int flag = 1;
    while (*str != '\0') {
        if (!isdigit(*str++)) {
            flag = 0;
            break;
        }
    }
    return flag;
}

void ListMnemonics(const char *str) {
    static char temp[50];
    static int n = 0;
    if (*str) {
        switch (*str) {
        case '0':
        case '1':
            temp[n++] = *str;
            ListMnemonics(str + 1);
            temp[--n] = '\0';
            break;
        default:
            for (int i = 0; i < 3; ++i) {
                if (n == 0 && i) { // 按照第一个字母分行
                    putchar('\n');
                }
                temp[n++] = table[*str - '0'][i];
                ListMnemonics(str + 1);
                temp[--n] = '\0';
            }
            break;
        }
    } else {
        printf("%s ", temp);
        return;
    }
}