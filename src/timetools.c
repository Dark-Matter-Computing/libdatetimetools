/*
 * =============================================================================
 *
 *       Filename:  timetools.c
 *
 *    Description:  This program has started with working exercises 5-5 and 5-6
 *                  in Practical C.  It will be modified into a library of time
 *                  calculating functions.
 *
 *        Version:  See version.h
 *        Created:  11/25/2020 21:15:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Thomas H. Vidal (THV), thomashvidal@gmail.com
 *   Organization:  Dark Matter Computing
 *  
 *      Copyright:  (c) 2011-2020 - Thomas H. Vidal
 *        License:  This file is part of libdatetimetools library.
 *
 *                  libdatetimetools is free software: you can redistribute it
 *                  and/or modify it under the terms of the GNU Lesser General
 *                  Public License as published by the Free Software Foundation,
 *                  version 3 of the License.
 *
 *                  libdatetimetools is distributed in the hope that it will be
 *                  useful,but WITHOUT ANY WARRANTY; without even the implied
 *                  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *                  PURPOSE.  See the GNU Lesser General Public License for
 *                  more details.
 *
 *                  You should have received a copy of the GNU General Public
 *                  License along with libdatetimetools.  If not, see
 *                  <https://www.gnu.org/licenses/>.
 *
 *	        Usage: Perform time computations.
 *    File Format: None incorporated as of August 18, 2011.
 *   Restrictions: 
 * Error Handling: 
 *     References: Practical C Programming, S. Oualline @ 83.
 *          Notes: 
 * 
 * SPDX-License-Identifier:  LGPL-3.0-only
 =============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/timetools.h"

/* global variables */
char line[100]; /* keyboard input buffer */
int hr, min, sec; /* hours, minutes, and seconds */
int ttlmin; /* total number of minutes */

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
