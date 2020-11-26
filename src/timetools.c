/*******************************************************************************
* Time Tools.c This program has started with working exercises 5-5 and 5-6 in  *
*               Practical C.  It will be modified into a library of time       *
*               calculating functions.                                         *
*                                                                              *
* Author: Thomas Vidal                                                         *
*                                                                              *
* Purpose: Practice the exercises in Practical C                               *
*                                                                              *
* Usage: Tests arrays, Qualifiers, and reading numbers                         *
*                                                                              *
* References: Practical C Programming, S. Oualline @ 83.                       *
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

/* global variables */
char line[100]; /* keyboard input buffer */
int hr, min, sec; /* hours, minutes, and seconds */
int ttlmin; /* total number of minutes */

/* function prototypes */
/* void parsetime (char input[], int *h, int *m, int *s); not used right now*/
int summin (int h, int m, int s);
void min2time (int min, int *h, int *m, int *s);

int main()
{
    /* Exercise 5-5 */
    printf("________________________________________________________\n");
    printf("Enter time in hh:m:ss format:\n");
    fgets(line, sizeof(line), stdin);
    sscanf(line, "%d:%d:%d", &hr, &min, &sec);
    printf("The total number of minutes is %d", summin(hr, min, sec));
    printf("\n________________________________________________________\n\n");


    /* Exercise 5-6 */
    printf("Enter the total number of minutes:\n");
    fgets(line, sizeof(line), stdin);
    sscanf(line, "%d", &ttlmin);
    min2time (ttlmin, &hr, &min, &sec);
    printf("The time is %d:%d:%d.\n", hr, min, sec);
    printf("\n________________________________________________________\n");

    return 0;
}

#ifdef UNDEF
void parsetime (char input[], int *h, int *m, int *s)
{
    int count; /*count for parsing loop */
    char buffer[100];

    if (input[0] >= 48 && input[0] <= 57)
    {

    }
    return;
}
#endif

int summin (int h, int m, int s)
{
    return ((h*60) + m + (s/60));
}

void min2time (int min, int *h, int *m, int *s)
{
    *h = min /60;
    *m = min - (*h*60);
    *s = 0;

    return;
}
