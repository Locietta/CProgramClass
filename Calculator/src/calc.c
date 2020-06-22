/**
 * @file calc.c
 * @author Locietta 程国斐 赵萌
 * @brief 计算相关的所有函数实现
 *
 * 包括表达式分析、中缀后缀转换、符号的优先级，操作数和结合性
 * 后缀表达式的计算
 * 另外一个重要的设计是错误信息的逐级传递
 *
 * @version 0.1
 * @date 2020-05-17
 *
 */

#include "calc.h"
#include "stack.h"
#include "queue.h"
#include "error.h"
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>

/**
 * Constants -- TYPE_NUM TYPE_OP TYPE_FUNC
 * -------------------------------------------
 * identifier of opNode::type
 */

enum { TYPE_NUM, TYPE_OP, TYPE_FUNC, TYPE_IDENTIFIER };

/**
 * Constants -- LEFT RIGHT
 * -------------------------------------------
 * identifier for op_associate()
 */

enum { LEFT, RIGHT };

/**
 * Data Structure -- opNode
 * Usage: queue exprQueue = newQueue(opNode);
 * -------------------------------------------
 * store operators and oprands(numbers)
 * opNode::type indicate the type it store (operator or oprand)
 */

typedef struct opNode {
    int type; // 0: op, 1: num
    union {   // C11 std: anoymous union
        char op[MAX_FUNC_NAME + 1];
        double num;
    };
} opNode;

/* Private Function Prototypes */

/* 1 -- overall steps */

static ERROR_INFO exprAnalyzer(const char *expr, queue *infixRes);

static ERROR_INFO convert2Postfix(queue infix, queue *postfixRes);

static ERROR_INFO calcPostfix(queue postfix, double *result);

/* 2 -- operator attributes */

static int op_prior(const char *op);

static int op_argnum(const char *op);

static int op_associate(const char *op);

/* 3 -- detailed procedures */

static ERROR_FLAG calcSingle(char *op, double num[], double *result);

static int bracketMatched(const char *expr);

static const char *numberFetcher(const char *expr, double *result, char *number);

static const char *functionFetcher(const char *expr, char *funcName);

/* 4 -- type judge */

static int isFunction(char *name);

#define isOperator(c)                                                                              \
    (c == '+' || c == '-' || c == '/' || c == '÷' || c == '*' || c == '%' || c == '=' || c == '^')

#define isLeftBrac(c) (c == '(' || c == '{' || c == '[')

#define isRightBrac(c) (c == ')' || c == '}' || c == ']')

#define isNum(c) (isdigit(c) || c == '.')

/* Function implementations */

/* Public functions */

ERROR_INFO Calc(char *expr, double *result) {
    queue infix, postfix;
    ERROR_INFO ret = {NOT_ERROR};
    if (*expr) {
        if ((ret = exprAnalyzer(expr, &infix)).error) {
            return ret;
        }
        if ((ret = convert2Postfix(infix, &postfix)).error) {
            return ret;
        }
        if ((ret = calcPostfix(postfix, result)).error) {
            return ret;
        }
    }
    return ret;
}

/* Private Functions */

static int op_prior(const char *op) {
    switch (strlen(op)) {
    case 1:
        switch (*op) {
        case '^': return 4;
        case '*':
        case '/':
        case '÷':
        case '%': return 3;
        case '+':
        case '-': return 2;
        case '=': return 1;
        default: return 0;
        }
        break;
    default: // functions have the highest prior
        return INT_MAX;
    }
}

static int op_argnum(const char *op) {
    switch (strlen(op)) {
    case 1:
        switch (*op) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '÷':
        case '%':
        case '^':
        case '=': // here '=' means 'compare'
            return 2;
        default: return -1; // unknown operator
        }
        break;
    default:
        if (!strcmp(op, "sin")) {
            return 1;
        } else if (!strcmp(op, "cos")) {
            return 1;
        } else if (!strcmp(op, "ln")) {
            return 1;
        } else if (!strcmp(op, "exp")) {
            return 1;
        } else if (!strcmp(op, "pow")) {
            return 2;
        } else {
            return -2; // unknown function
        }
    }
}

static int op_associate(const char *op) {
    if (!strcmp(op, "^")) {
        return RIGHT;
    } else {
        return LEFT;
    }
}

static ERROR_FLAG calcSingle(char *op, double num[], double *result) {
    switch (strlen(op)) {
    case 1:
        switch (*op) {
        case '+': *result = num[0] + num[1]; break;
        case '-': *result = num[0] - num[1]; break;
        case '*': *result = num[0] * num[1]; break;
        case '/':
        case '÷':
            if (num[1]) {
                *result = num[0] / num[1];
            } else {
                // puts("0 can't be used as divide number")
                return ERROR_DIVIDE_ZERO;
            }
            break;
        case '%':
            if ((int) num[0] == num[0] && (int) num[1] == num[1]) {
                *result = (int) num[0] % (int) num[1];
            } else {
                // puts("% operation demands integers");
                return ERROR_MOD_NONINTERGER;
            }
            break;
        case '^': *result = pow(num[0], num[1]); break;
        case '=': *result = (num[0] == num[1]); break;
        }
        break;
    default:
        if (!strcmp(op, "sin")) {
            *result = sin(num[0]);
        } else if (!strcmp(op, "cos")) {
            *result = cos(num[0]);
        } else if (!strcmp(op, "ln")) {
            if (num[0] <= 0) {
                return ERROR_NEGATIVE_IN_LOG;
            }
            *result = log(num[0]);
        } else if (!strcmp(op, "exp")) {
            *result = exp(num[0]);
        } else if (!strcmp(op, "pow")) {
            *result = pow(num[0], num[1]);
        }
        break;
    }
    return NOT_ERROR;
}

static ERROR_INFO exprAnalyzer(const char *expr, queue *infixRes) {

    if (!bracketMatched(expr)) {
        // puts("brackets unmatched");
        return (ERROR_INFO){ERROR_UNMATCHED_BRAC};
    }

    // /* this if-block is not neccessary */
    // if (!isalnum(*expr) && !isLeftBrac(*expr) && *expr == '.') {
    //     // puts("not begin with number, function or parenthese");
    //     return 2;
    // }

    queue infix = newQueue(opNode);
    const char *start = expr;
    while (*expr) {
        if (isNum(*expr) || ((*expr == '-' || *expr == '+') &&
                             ((!isdigit(*(expr - 1)) && isNum(*(expr + 1))) || expr == start))) {
            opNode tempNode = {TYPE_NUM, .num = 0};
            char numChar[MAX_NUMBER + 1];
            if ((expr = numberFetcher(expr, &tempNode.num, numChar))) {
                infix.push(&infix, &tempNode);
            } else {
                // puts("invalid number");
                infix.destory(&infix);
                ERROR_INFO ret = {ERROR_INVALID_NUMBER};
                ret.number = (char *) malloc((MAX_NUMBER + 1) * sizeof(char));
                strcpy(ret.number, numChar);
                return ret;
            }
        } else if (isalpha(*expr)) {
            opNode tempNode = {TYPE_FUNC, .op = ""};
            if ((expr = functionFetcher(expr, tempNode.op))) {
                infix.push(&infix, &tempNode);
            } else {
                // puts("unknown function name");
                infix.destory(&infix);
                ERROR_INFO ret = {ERROR_UNKNOWN_FUNC_NAME};
                ret.funcName = (char *) malloc((MAX_FUNC_NAME + 1) * sizeof(char));
                strcpy(ret.funcName, tempNode.op);
                return ret;
            }
        } else if (isOperator(*expr)) {
            opNode tempNode = {TYPE_OP, .op = ""};
            tempNode.op[0] = *(expr++);
            tempNode.op[1] = '\0';
            infix.push(&infix, &tempNode);
        } else if (*expr == ',' || isLeftBrac(*expr) || isRightBrac(*expr)) {
            opNode tempNode = {TYPE_IDENTIFIER, .op = ""};
            tempNode.op[0] = *(expr++);
            tempNode.op[1] = '\0';
            infix.push(&infix, &tempNode);
        } else {
            // puts("unknown operator");
            infix.destory(&infix);
            return (ERROR_INFO){ERROR_UNKNOWN_OP, .op = *expr};
        }
    }
    *infixRes = infix;
    return SUCCESS;
}

static const char *numberFetcher(const char *expr, double *result, char *number) {
    char buf[MAX_NUMBER + 1], *numstr = buf, error = 0;
    const char *start = expr;
    if (*expr == '+' || *expr == '-') {
        *(numstr++) = *(expr++);
    }
    if (*expr == '.') {
        *(numstr++) = '0';
    }
    while (isdigit(*expr) || *expr == '.') {
        if (isdigit(*expr)) {
            *(numstr++) = *(expr++);
        } else if (expr != start) { // && *expr == '.'
            char *temp = numstr - 1, flag = 0;
            while (temp >= buf) {
                if (*(temp--) == '.') {
                    flag = 1;
                    break;
                }
            }
            *(numstr++) = *(expr++);
            if (flag) {
                error = 1; // one number with two (or more) '.' e.g. 1.23.5
            }
        } else {
            *(numstr++) = *(expr++);
        }
    }
    *numstr = '\0';
    strcpy(number, buf);
    sscanf(buf, "%lf", result);
    return error ? NULL : expr;
}

static int isFunction(char *name) {
    return !(strcmp(name, "sin") && strcmp(name, "cos") && strcmp(name, "ln") &&
             strcmp(name, "exp") && strcmp(name, "pow"));
}

static const char *functionFetcher(const char *expr, char *funcName) {
    char buf[MAX_FUNC_NAME + 1], *funcstr = buf;
    while (isalpha(*expr)) {
        *(funcstr++) = *(expr++);
    }
    *funcstr = '\0';
    strcpy(funcName, buf);
    if (isFunction(buf)) {
        return expr;
    } else {
        return NULL;
    }
}

static int bracketMatched(const char *expr) {
    stack brac_stack = newStack(char);
    char buf;
    while (*expr != '\0') {
        switch (*expr) {
        case '{':
        case '[':
        case '(': brac_stack.push(&brac_stack, expr); break;
        case '}':
        case ']':
            if (brac_stack.pop(&brac_stack, &buf) == POP_IN_EMPTY_STACK || buf + 2 != *expr) {
                brac_stack.destory(&brac_stack);
                return 0;
            }
            break;
        case ')':
            if (brac_stack.pop(&brac_stack, &buf) == POP_IN_EMPTY_STACK || buf != '(') {
                brac_stack.destory(&brac_stack);
                return 0;
            }
            break;
        }
        ++expr;
    }
    if (brac_stack.empty(&brac_stack)) {
        brac_stack.destory(&brac_stack);
        return 1;
    } else {
        brac_stack.destory(&brac_stack);
        return 0;
    }
}

static ERROR_INFO convert2Postfix(queue infix, queue *postfixRes) {
    queue postfix = newQueue(opNode);
    stack opstack = newStack(opNode);

    while (!infix.empty(&infix)) {
        opNode tempNode;
        infix.pop(&infix, &tempNode);

        if (tempNode.type == TYPE_NUM) {
            postfix.push(&postfix, &tempNode);
        } else if (tempNode.type == TYPE_FUNC) {
            opstack.push(&opstack, &tempNode);
        } else if (!strcmp(tempNode.op, ",")) {
            while (!opstack.empty(&opstack)) {
                opNode optemp;
                opstack.pop(&opstack, &optemp);
                if (isLeftBrac(optemp.op[0])) {
                    opstack.push(&opstack, &optemp);
                    break;
                } else {
                    postfix.push(&postfix, &optemp);
                }
            }
            if (opstack.empty(&opstack)) {
                // puts("wrong postion for function argument spliter ','");
                postfix.destory(&postfix); // caution leakage
                opstack.destory(&opstack);
                infix.destory(&infix);

                return (ERROR_INFO){ERROR_WRONG_POSITION_ARGUMENT_SPLITER};
            }
        } else if (tempNode.type == TYPE_OP) {
            while (!opstack.empty(&opstack)) {
                if ((op_prior(((opNode *) opstack.top(&opstack))->op) > op_prior(tempNode.op)) ||
                    (op_prior(((opNode *) opstack.top(&opstack))->op) == op_prior(tempNode.op) &&
                     op_associate(tempNode.op) == LEFT)) {

                    opNode optemp;
                    opstack.pop(&opstack, &optemp);
                    postfix.push(&postfix, &optemp);
                } else {
                    break;
                }
            }
            opstack.push(&opstack, &tempNode);
        } else if (isLeftBrac(tempNode.op[0])) {
            opstack.push(&opstack, &tempNode);
        } else if (isRightBrac(tempNode.op[0])) {
            while (!opstack.empty(&opstack) &&
                   !isLeftBrac(((opNode *) opstack.top(&opstack))->op[0])) {
                opNode optemp;
                opstack.pop(&opstack, &optemp);
                postfix.push(&postfix, &optemp);
            }
            opstack.pop(&opstack, NULL); // clean left bracket
            // if there is no left bracket, there must be somthing wrong with bracketMatched()
        }
    }
    infix.destory(&infix);
    while (!opstack.empty(&opstack)) {
        opNode tempop;
        opstack.pop(&opstack, &tempop);
        postfix.push(&postfix, &tempop);
    }
    opstack.destory(&opstack);

    *postfixRes = postfix;
    return SUCCESS;
}

static ERROR_INFO calcPostfix(queue postfix, double *result) {
    stack calcstack = newStack(double);
    opNode tempNode;
    while (postfix.pop(&postfix, &tempNode) != POP_IN_EMPTY_QUEUE) {
        if (tempNode.type == TYPE_NUM) {
            calcstack.push(&calcstack, &tempNode.num);
        } else {
            unsigned int arg_needNum = op_argnum(
                tempNode
                    .op); // unsigned is neccessary OR you'll get a wierd error with gcc8 or newer
            double num[arg_needNum]; // C99: VLA
            for (int i = 1; i <= arg_needNum; ++i) {
                if (!calcstack.empty(&calcstack)) {
                    calcstack.pop(&calcstack, num + arg_needNum - i);
                } else {
                    // puts("argument number unmatched");
                    postfix.destory(&postfix);
                    calcstack.destory(&calcstack);
                    return (ERROR_INFO){ERROR_WRONG_OPRAND_NUMBER};
                }
            }
            double temp;
            ERROR_INFO ret = {NOT_ERROR};
            if ((ret.error = calcSingle(tempNode.op, num, &temp))) {
                postfix.destory(&postfix);
                calcstack.destory(&calcstack);
                return ret;
            } else {
                calcstack.push(&calcstack, &temp);
            }
        }
    }
    if (calcstack.size == 1) {
        calcstack.pop(&calcstack, result);
    } else if (calcstack.size > 1) {
        // puts("argument number unmatched");
        postfix.destory(&postfix);
        calcstack.destory(&calcstack);
        return (ERROR_INFO){ERROR_WRONG_OPRAND_NUMBER};
    }
    postfix.destory(&postfix);
    calcstack.destory(&calcstack);
    return SUCCESS;
}
