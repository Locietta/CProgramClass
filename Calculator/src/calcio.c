#include "calcio.h"
#include <string.h>

/* Private Function Prototypes */

char *vExpr_gets(char *s, int n, FILE *stream);

/* Function implementations */

/* Public functions */

char *Expr_gets(char *st, int n) {
    register char *ret_val;
    register char *find;

    ret_val = vExpr_gets(st, n, stdin);
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

/* Private Function */

char *vExpr_gets(char *s, int n, FILE *stream) {
    register int c;
    register char *cs;
    cs = s;

    while (--n > 0 && (c = tolower(getc(stream))) != EOF) { // no difference if you use CAPITALS
        if (c == ' ' || c == '\t') {    // ignore all the ' ' or '\t'
            ++n;
            continue;
        } else if ((*cs++ = c) == '\n') {
            break;
        }
    }

    *cs = '\0';
    return (c == EOF && cs == s) ? NULL : s;
}
