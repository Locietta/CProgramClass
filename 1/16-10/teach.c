/*
 * File: teach.c
 * -------------
 * This program executes a simple programmed instruction course.
 * The course is specified by a data file containing all the
 * course information.  The data structures and the format of
 * the data file are described in Chapter 16.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "genlib.h"
#include "simpio.h"
#include "strlib.h"

/*
 * Constants
 * ---------
 * MaxQuestions          -- Maximum question number
 * MaxLinesPerDesciption -- Maximum number of lines per description
 * MaxLinesPerQuestion   -- Maximum number of lines per question
 * MaxAnswersPerQuestion -- Maximum answers per question
 * EndMarker             -- String marking end of question text
 */

#define MaxQuestions 100
#define MaxLinesPerDesciption 20
#define MaxLinesPerQuestion 20
#define MaxAnswersPerQuestion 10
#define EndMarker "-----"

/* Data structures */

/*
 * Type: answerT
 * -------------
 * This structure provides space for each possible answer
 * to a question.
 */

typedef struct {
    string ans;
    int nextq;
} answerT;

/*
 * Type: questionT
 * ---------------
 * This structure provides space for all the information
 * needed to store one of the individual question records.
 * Because this structure is large and it makes sense
 * to refer to it as a single entity, questionT is defined
 * as a pointer type.
 */

typedef struct {
    string qdescription[MaxLinesPerDesciption + 1];
    string qtext[MaxLinesPerQuestion + 1];
    answerT answers[MaxAnswersPerQuestion];
    int nAnswers;
} * questionT;

/*
 * Type: courseDB
 * --------------
 * This type is used to define the entire database, which is
 * a pointer to a record containing the title and an array of
 * questions.
 */

typedef struct {
    string title;
    questionT questions[MaxQuestions + 1];
} * courseDB;

/* Private function declarations */

static courseDB ReadDataBase(void);
static bool ReadOneQuestion(FILE *infile, courseDB course);
static void ReadQuestionDescription(FILE *infile, questionT q);
static void ReadQuestionText(FILE *infile, questionT q);
static void ReadAnswers(FILE *infile, questionT q);
static FILE *OpenUserFile(string prompt, string mode);
static void ProcessCourse(courseDB course);
static void AskQuestion(questionT q, int asktimes);
static int FindAnswer(string ans, questionT q);

/* Main program */

int main(void) {
    courseDB course;

    course = ReadDataBase();
    ProcessCourse(course);
    return 0;
}

/* Section 1 -- Functions to read the data file */

/*
 * Function: ReadDataBase
 * Usage: ReadDataBase();
 * ----------------------
 * This function asks the user for a file name and reads
 * in the database for the course.  The file is formatted
 * as discussed in the section "Designing the external
 * structure" in Chapter 16.
 */

static courseDB ReadDataBase(void) {
    FILE *infile;
    courseDB course;

    infile = OpenUserFile("Enter name of course: ", "r");
    course = New(courseDB);
    course->title = ReadLine(infile);
    while (ReadOneQuestion(infile, course)) continue;
    fclose(infile);
    return (course);
}

/*
 * Function: ReadOneQuestion
 * Usage: while (ReadOneQuestion(infile, course));
 * -----------------------------------------------
 * This function reads in a single question from infile into the
 * course data structure.  As long as the complete question is
 * read successfully, this function returns TRUE.  When the end
 * of the file is encountered, the function returns FALSE.
 * Thus, the "Usage" line above reads the entire data file.
 */

static bool ReadOneQuestion(FILE *infile, courseDB course) {
    questionT question;
    string line;
    int qnum;

    line = ReadLine(infile);
    if (line == NULL) return (FALSE);
    qnum = StringToInteger(line);
    if (qnum < 1 || qnum > MaxQuestions) {
        Error("Question number %d out of range", qnum);
    }
    question = New(questionT);
    ReadQuestionDescription(infile, question);
    ReadQuestionText(infile, question);
    ReadAnswers(infile, question);
    course->questions[qnum] = question;
    return (TRUE);
}

/*
 * Function: ReadQuestionDescription
 * Usage: ReadQuestionDescription(infile, question);
 * ------------------------------------------
 * This function reads the description of the question into the
 * question data structure, which must have been allocated
 * by the caller.  The end of the question description is signaled
 * by a line matching the string EndMarker.
 */

static void ReadQuestionDescription(FILE *infile, questionT q) {
    string line;
    int nlines;

    nlines = 0;
    while (TRUE) {
        line = ReadLine(infile);
        if (StringEqual(line, EndMarker)) break;
        if (nlines == MaxLinesPerDesciption) {
            Error("Too many lines");
        }
        q->qdescription[nlines] = line;
        nlines++;
    }
    q->qdescription[nlines] = NULL;
}

/*
 * Function: ReadQuestionText
 * Usage: ReadQuestionText(infile, question);
 * ------------------------------------------
 * This function reads the text of the question into the
 * question data structure, which must have been allocated
 * by the caller.  The end of the question text is signaled
 * by a line matching the string EndMarker.
 */

static void ReadQuestionText(FILE *infile, questionT q) {
    string line;
    int nlines;

    nlines = 0;
    while (TRUE) {
        line = ReadLine(infile);
        if (StringEqual(line, EndMarker)) break;
        if (nlines == MaxLinesPerQuestion) {
            Error("Too many lines");
        }
        q->qtext[nlines] = line;
        nlines++;
    }
    q->qtext[nlines] = NULL;
}

/*
 * Function: ReadAnswers
 * Usage: ReadAnswers(infile, question);
 * -------------------------------------
 * This function reads the answer pairs for the question
 * from the input file.  Each answer consists of a string
 * followed by a colon, followed by the number of the next
 * question to be read.  The end of the answer list is
 * signaled by a blank line or the end of the file.
 */

static void ReadAnswers(FILE *infile, questionT q) {
    string line, ans;
    int len, cpos, nextq, nAnswers;

    nAnswers = 0;
    while ((line = ReadLine(infile)) != NULL && (len = StringLength(line)) != 0) {
        cpos = FindChar(':', line, 0);
        if (cpos == -1) Error("Illegal answer format");
        ans = SubString(line, 0, cpos - 1);
        nextq = StringToInteger(SubString(line, cpos + 1, len - 1));
        q->answers[nAnswers].ans = ConvertToUpperCase(ans);
        q->answers[nAnswers].nextq = nextq;
        nAnswers++;
    }
    q->nAnswers = nAnswers;
}

/*
 * Function: OpenUserFile
 * Usage: fileptr = OpenUserFile(prompt, mode);
 * --------------------------------------------
 * This function prompts the user for a file name using the
 * prompt string supplied by the user and then attempts to
 * open that file with the specified mode.  If the file is
 * opened successfully, OpenUserFile returns the appropriate
 * file pointer.  If the open operation fails, the user is
 * informed of the failure and given an opportunity to enter
 * another file name.
 */

static FILE *OpenUserFile(string prompt, string mode) {
    string filename;
    FILE *result;

    while (TRUE) {
        printf("%s", prompt);
        filename = GetLine();
        result = fopen(filename, mode);
        if (result != NULL) break;
        printf("Can't open the file \"%s\"\n", filename);
    }
    return (result);
}

/* Section 2 -- Functions to process the course */

/*
 * Function: ProcessCourse
 * Usage: ProcessCourse(course);
 * -----------------------------
 * This function processes the course supplied by the caller.
 * The basic operation consists of a loop that
 *
 *    (a) prints out the current question
 *    (b) reads in an answer
 *    (c) looks up the answer in the database
 *    (d) goes to a new question on the basis of that answer
 *
 * In this implementation, the variable qnum holds the
 * index of the question and the variable q holds the
 * actual question data structure.  The course always begins
 * with question #1, after which the order is determined by
 * the answers.
 */

static void ProcessCourse(courseDB course) {
    questionT q;
    int qnum;
    string ans;
    int asktimes = 0, index;

    printf("%s\n", course->title);
    qnum = 1;
    while (qnum != 0) {
        q = course->questions[qnum];
        AskQuestion(q, asktimes);
        ans = ConvertToUpperCase(GetLine());
        index = FindAnswer(ans, q);
        if (index == -1) {
            printf("I don't understand that.\n");
            asktimes++;
        } else {
            qnum = q->answers[index].nextq;
            asktimes = 0;
        }
    }
}

/*
 * Function: AskQuestion
 * Usage: AskQuestion(q);
 * ----------------------
 * This function asks the question indicated by the questionT
 * specified by q.  Asking the question consists of displaying
 * each of the lines that comprise the question text.
 */

static void AskQuestion(questionT q, int asktimes) {
    if (!asktimes) {
        for (int i = 0; q->qdescription[i] != NULL; i++) {
            printf("%s\n", q->qdescription[i]);
        }
    }
    for (int i = 0; q->qtext[i] != NULL; i++) {
        printf("%s\n", q->qtext[i]);
    }
}

/*
 * Function: FindAnswer
 * Usage: FindAnswer(ans, q)
 * -------------------------
 * This function looks up the string ans in the list of answers
 * for question q.  If the answer is found, its index in the
 * answer list is returned.  If not, the function returns -1.
 * The function uses a simple linear search algorithm to look
 * through the array.
 */

static int FindAnswer(string ans, questionT q) {
    for (int i = 0; i < q->nAnswers; i++) {
        if (StringEqual(ans, q->answers[i].ans)) return (i);
    }
    return (-1);
}