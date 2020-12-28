/*
 * Filename:  timetools.c
 *
 * FOR DESCRIPTION AND OTHER DETAILS, PLEASE SEE THE TIMETOOLS.H AND 
 * DATETIMETOOLS_PVT.H header files.
 *
 * Version:  See version.h
 * Created:  11/25/2020 21:15:15
 * Last Modified: Sun Dec 27 15:00:16 2020
 *
 * Author: Thomas H. Vidal (THV), thomashvidal@gmail.com
 * Organization: Dark Matter Computing
 *
 * Copyright: (c) 2011-2020 - Thomas H. Vidal, Los Angeles, CA
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/timetools.h"

/* global variables */
char line[100]; /* keyboard input buffer */
int hr, min, sec; /* hours, minutes, and seconds */
int ttlmin; /* total number of minutes */


#ifdef UNDEF
int testfunc()
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
