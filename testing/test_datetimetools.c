/*
 * Filename: test_datetimetools.c
 * Library: libdatetimetools
 *
 * Version: 1.0.0.0
 * Created: Mon Dec 21 21:08:29 2020
 * Last Modified: Fri Feb  5 17:33:51 2021
 *
 * Author: Thomas H. Vidal (THV), thomashvidal@gmail.com
 * Organization: Dark Matter Computing
 *  
 * Copyright: Copyright (c) 2011-2020, Thomas H. Vidal
 * SPDX-License-Identifier: LGPL-3.0-only
 *
 * Usage: 
 *
 * File Format: 
 * Restrictions: 
 * Error Handling: 
 * References: 
 * Notes:
 */

/* #####   HEADER FILE INCLUDES   ########################################### */

#include <stdio.h>
#include <stdlib.h>
#include "../include/datetools.h"
#include "../include/timetools.h"
#include "testsuite.h"

FILE *HOLIDAY_FILE;

void usage(char *program_name);

int main(int argc, char *argv[])
{
    char *program_name;
    char *holidays_filename;
    char *weekdaytest_filename;
    char *datecalc_filename;
    char *leapcalc_filename;
    char *rulecheck_filename;
    char *mathcalc_filename;
    char *calmath_filename;
    int close_file_when_done = 1;
    

    /* initialize file names */
    program_name = argv[0];
    holidays_filename = NULL;
    weekdaytest_filename = NULL;
    datecalc_filename = NULL;
    leapcalc_filename = NULL;
    rulecheck_filename = NULL;
    mathcalc_filename = NULL;
    calmath_filename = NULL;

    /* Process the commandline arguments */
    if (argc == 1) {
        usage(program_name);
        return -1;
    }
    while ((argc > 1) && (argv[1][0] == '-'))
    {
        switch (argv[1][1])
        {
            case 'H': /* fall through */
            case 'h':
                holidays_filename = &argv[1][2];
                holiday_rules_open(holidays_filename, close_file_when_done);
                printholidayrules();
                break;
            case 'C': /* fall through */
            case 'c':
                datecalc_filename = &argv[1][2];
                testsuite_run_check(CALCS, datecalc_filename);
                break;
            case 'I': /* fall through */
            case 'i':
                testsuite_interactive();
                break;
            case 'L': /* fall through */
            case 'l':
                leapcalc_filename = &argv[1][2];
                testsuite_run_check(LEAPDATES, leapcalc_filename);
                break;
            case 'K': /* fall through */
            case 'k':
                calmath_filename = &argv[1][2];
                testsuite_run_check(CALMATH, calmath_filename);
                break;
            case 'M': /* fall through */
            case 'm':
                mathcalc_filename = &argv[1][2];
                testsuite_run_check(MATHCALC, mathcalc_filename);
            case 'R': /* fall through */
            case 'r':
                rulecheck_filename = &argv[1][2];
                testsuite_run_check(RULECHECK, rulecheck_filename);
                break;
            case 'W': /* fall through */ 
            case 'w':
                weekdaytest_filename = &argv[1][2];
                testsuite_run_check(DERIVEDATES, weekdaytest_filename);
                break;
            default:
                fprintf(stderr, "Bad option %s\n", argv[1]);
                usage(program_name);
        }
        ++argv;
        --argc;
    }
    /* testsuite(); */
    return 0;
}


/* 
 * Description:  Called when there is an error on the commandline
 * Parameters:  Char pointer to the program's name
 * Returns:  Exit 8
 * Algorithm:  
 * References:   
 * Notes:   
 */

void usage(char *program_name)
{
    int padding = (int) strlen(program_name);
    
    printf("In Function: Usage\n");
    fprintf(stderr, "Uasge is %s -hcilrw\n",
            program_name);
    
    fprintf(stderr, "%-32s", " ");
    fprintf(stderr, "-i -> interactive mode\n");
    fprintf(stderr, "%-32s", " ");
    fprintf(stderr, "-h[holiday rules filename]\n");
    fprintf(stderr, "%-32s", " ");
    fprintf(stderr, "-l[leap year test filename]\n");
    exit(8);
}
