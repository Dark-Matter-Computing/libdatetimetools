/*
 * Filename: testsuite.c
 * Library: libdatetimetools
 *
 * Description:  Testsuite.c creates a number of functions for testing the
 * libdatetimetools library's functionality and quality.
 *
 * Version: 1.0.0.0
 * Created:  Mon Dec 21 21:17:02 2020
 * Last Modified: Mon Jan  4 18:49:04 2021
 *
 * Author: Thomas H. Vidal (THV), thomashvidal@gmail.com
 * Organization: Dark Matter Computing
 *  
 * Copyright: Copyright (c) 2011-2020, Thomas H. Vidal
 * SPDX-License-Identifier: LGPL-3.0-only
 *
 * Usage: Currently the testsuite functions are called from main.
 * File Format: None
 * Restrictions: --
 * Error Handling: --
 * References: --
 * Notes: --
 */

/* #####   HEADER FILE INCLUDES   ########################################### */
#include <stdlib.h>
#include <errno.h>
#include "../include/datetools.h"
#include "../include/timetools.h"
#include "testsuite.h"

/* Output Constants */
#define MAXMESSAGELEN 204 /* Enough for 3 lines */
const int   SCREENWIDTH     = 70;
const int   FRAMESIDEWIDTH  = 1;
const char  FRAME_SYMB      = '*';

enum FRAMEPOS {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    SECTIONBREAK
};

enum TESTPHASE{
    BUILD_FRAME,
    TESTING,
    REPORT_RESULTS,
    BLANK_ROW,
    EMPTY_ROW,
    END_FRAME
};

/* Functions */

void testsuite_interactive(void)
{
    struct DateTime begin_date; 
    struct DateTime end_date; 
    /*  struct DateTime result_date; */
    int day_count; 
    /*  char dateinput[11]; */
    char line[80]; 

    printf("Test your own dates.\n");
    printf("please enter the beginning date in the format mm/dd/yyyy");
    printf("\n(Press x to end):\n");

    fgets(line, sizeof(line), stdin);
    sscanf(line, "%d/%d/%d", &begin_date.month, &begin_date.day,
           &begin_date.year);
    printf("\nHow many days out is the deadline?");
    fgets(line, sizeof(line), stdin);
    sscanf(line, "%d", &day_count);
    date_offset (&begin_date, &end_date,
                  day_count);
    printf("The deadline is: %d/%d/%d.\n", end_date.month, end_date.day,
           end_date.year);
    if (isholiday(&end_date)==0)
        printf("The end date is valid and does not fall on a holiday.\n");
    else
        printf("The end date is NOT valid.  It falls on a holiday.\n");
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    return;
}

void testsuite_run_check(enum TESTFILETYPES testtype, const char *testfile_name)
{
    
    FILE *testfile;
    
    switch (testtype) {
        case DERIVEDATES:
            testfile = fopen(testfile_name, "r");
            if (testfile == NULL) {
                fprintf (stderr, "couldn't open file '%s'; %s\n",
                         testfile_name, strerror(errno));
                exit (EXIT_FAILURE);
            }
            testsuite_check_deriveday(testfile);
            fclose(testfile);
            break;
        case CALCS:
            testfile = fopen(testfile_name, "r");
            if (testfile == NULL) {
                fprintf (stderr, "couldn't open file '%s'; %s\n",
                         testfile_name, strerror(errno));
                exit (EXIT_FAILURE);
            }
            testsuite_check_calcs(testfile);
            fclose(testfile);
            break;
        case LEAPDATES:
            testfile = fopen(testfile_name, "r");
            if (testfile == NULL) {
                fprintf (stderr, "couldn't open file '%s'; %s\n",
                         testfile_name, strerror(errno));
                exit (EXIT_FAILURE);
            }
            testsuite_check_leap(testfile);
            fclose(testfile);
            break;
        default:
            /* do nothing */
            break;
    }
    return;
}

void testsuite_check_calcs(FILE *openedtestfile)
{
    struct DateTime testdate;
    char line[100];
    int count;
    int result = 0;
    int truefalse = 0;
    int expected_result_A = 0;
    int expected_result_B = 0;
    char datestring[DATESTRINGLEN];
    char message[MAXMESSAGELEN];
    struct teststats calc_stats;

    calc_stats.ttl_tests = 0;
    calc_stats.successful_tests = 0;

    display_results(NULL, EMPTY_ROW);
    display_results("Date Calculation Functions", BUILD_FRAME);
    
    /* read and discard the headers */
    fgets(line, sizeof(line), openedtestfile);
    
    while (fgets(line, sizeof(line), openedtestfile) != NULL) {
        count = sscanf(line, "%d,%d,%d,%d,%d", &testdate.year,
                       &testdate.month, &testdate.day,
                       &expected_result_A, &expected_result_B);

        date_to_string(datestring, &testdate, MDY);
        testdate.day_of_week = derive_weekday(&testdate);
        result = derive_weekday(&testdate);

        sprintf(message, "Testing whether %s is that the last %s of %s...",
                datestring, wkday_to_string(testdate.day_of_week),
                month_to_string(testdate.month));

        display_results(message, TESTING);

        truefalse = islastxdom(&testdate);
        calc_stats.ttl_tests++;

        if(truefalse)
            sprintf(message, "    Yes. It is the last %s of the %s.",
                   wkday_to_string(testdate.day_of_week),
                   month_to_string(testdate.month));
        else
            sprintf(message, "    No. It is NOT last %s of %s.",
                   wkday_to_string(testdate.day_of_week),
                   month_to_string(testdate.month));

        if ((expected_result_A == 1) &&
            (truefalse == expected_result_A)) {
            calc_stats.successful_tests++;
            message_right_justify(message, "PASS", SCREENWIDTH);
        } else if ((expected_result_A == 0) &&
                   (truefalse == expected_result_A)) {
            calc_stats.successful_tests++;
            message_right_justify(message, "PASS", SCREENWIDTH);
        } else {
            message_right_justify(message, "FAIL", SCREENWIDTH);
        }

        display_results(message, TESTING);

        sprintf(message, "Now testing whether %s is in the last week of "
                "the month...", datestring);

        display_results(message, TESTING);
        truefalse = islastweek(&testdate);
        calc_stats.ttl_tests++;
        
        if(truefalse)
            sprintf(message, "    Yes. It is in the last week of %s",
                    month_to_string(testdate.month));
        else
            sprintf(message, "    No. It is NOT in the last week of %s.",
                    month_to_string(testdate.month));

        if ((expected_result_B == 1) &&
            (truefalse == expected_result_B)) {
            calc_stats.successful_tests++;
            message_right_justify(message, "PASS", SCREENWIDTH);
        } else if ((expected_result_B == 0) &&
                  (truefalse == expected_result_B)) {
            calc_stats.successful_tests++;
            message_right_justify(message, "PASS", SCREENWIDTH);
        } else {
            message_right_justify(message, "FAIL", SCREENWIDTH);
        }
        
        display_results(message, TESTING);
        display_results(NULL, BLANK_ROW);
    }
    display_stats(&calc_stats);
    display_results(NULL, END_FRAME);
    
    return;
}

void testsuite_check_deriveday(FILE *openedtestfile)
{
    struct DateTime testdate;
    char line[100];
    int count;
    int result = 0;
    int expected_result_A = 0;
    char datestring[DATESTRINGLEN];
    char message[MAXMESSAGELEN];
    struct teststats deriveday_stats;

    deriveday_stats.ttl_tests = 0;
    deriveday_stats.successful_tests = 0;
    
    display_results(NULL, EMPTY_ROW);
    display_results("Derive Day of Week Function", BUILD_FRAME);
    
    /* read and discard the headers */
    fgets(line, sizeof(line), openedtestfile);
    
    while (fgets(line, sizeof(line), openedtestfile) != NULL) {
        count = sscanf(line, "%d,%d,%d,%d", &testdate.year,
                      &testdate.month, &testdate.day,
                      &expected_result_A);

        result = derive_weekday(&testdate);
        deriveday_stats.ttl_tests++;

        date_to_string(datestring, &testdate, MDY);
        sprintf(message, "Testing the following (D/M/Y) %s ...",
                datestring);
        display_results(message, TESTING);

        if (result >= 0){
            sprintf(message, "    In range. Weekday is %s.",
                    wkday_to_string(result));
        } else {
            sprintf(message, "    Out of range. "
                   "No computation performed.");
        }

        if ((expected_result_A == -1) &&
            (result == expected_result_A)) {
            deriveday_stats.successful_tests++;
            message_right_justify(message, "PASS", SCREENWIDTH);
        } else if ((expected_result_A == 0) && (result >= 0)) {
            deriveday_stats.successful_tests++;
            message_right_justify(message, "PASS", SCREENWIDTH);
        } else {
            message_right_justify(message, "FAIL", SCREENWIDTH);
        }
        display_results(message, TESTING);
    }
    display_stats(&deriveday_stats);
    display_results(NULL, END_FRAME);
    return;
}

void testsuite_check_holidays(FILE *openedtestfile)
{
    struct DateTime testdate;
    char line[100];
    char ruletype;
    int count;
    int expected_result_A = 0;
    char datestring[DATESTRINGLEN];
    char message[MAXMESSAGELEN];
    struct teststats rulecheck_stats;

    rulecheck_stats.ttl_tests = 0;
    rulecheck_stats.successful_tests = 0;

    display_results(NULL, EMPTY_ROW);
    display_results("Holiday Rule-Checking Algorithm", BUILD_FRAME);
    
    /* read and discard the headers */
    fgets(line, sizeof(line), openedtestfile);
 
    while (fgets(line, sizeof(line), openedtestfile) != NULL) {
        count = sscanf(line, "%d,%d,%d,%c,%d", &testdate.year,
                       &testdate.month, &testdate.day,
                       &ruletype, &expected_result_A);
        
        date_to_string(datestring, &testdate, MDY);
        switch (ruletype) {
            case 'A': /* fall through */
            case 'a':
                sprintf(message, "Testing %s, an absolute rule...",
                        datestring);
                break;
            case 'R': /* fall through */
            case 'r':
                sprintf(message, "Testing %s, a relative rule...",
                        datestring);
                break;
            case 'W': /* fall through */
            case 'w':
                sprintf(message, "Testing %s, a weekend rule...",
                        datestring);
                break;
            default: /* do nothing */
                break;
        }
        rulecheck_stats.ttl_tests++;
        
        if (isholiday(&testdate) != 0){
            strcat(message, "    YES date is a holiday. ");
            if (expected_result_A == 1) {
                rulecheck_stats.successful_tests++;
                message_right_justify(message, "PASS", SCREENWIDTH);
            } else {
                message_right_justify(message, "FAIL", SCREENWIDTH);
            }
        } else {
            strcat(message, "    NO date is a holiday. ");
            if (expected_result_A == 1) {
                rulecheck_stats.successful_tests++;
                message_right_justify(message, "PASS", SCREENWIDTH);
            } else {
                message_right_justify(message, "FAIL", SCREENWIDTH);
            }
        }
        
        display_results(message, TESTING);
    }
    display_stats(&rulecheck_stats);
    display_results(NULL, END_FRAME);

    return;
}

void testsuite_check_leap(FILE *openedtestfile)
{
    struct DateTime testdate;
    char line[100];
    int count;
    int truefalse = 0;
    int expected_result_A = 0;
    char datestring[DATESTRINGLEN];
    char message[MAXMESSAGELEN];
    struct teststats leap_stats;

    leap_stats.ttl_tests = 0;
    leap_stats.successful_tests = 0;

    display_results(NULL, EMPTY_ROW);
    display_results("Leap Year Function", BUILD_FRAME);
    
    /* read and discard the headers */
    fgets(line, sizeof(line), openedtestfile);
    
    while (fgets(line, sizeof(line), openedtestfile) != NULL) {
        count = sscanf(line, "%d,%d,%d,%d", &testdate.year,
                      &testdate.month, &testdate.day,
                      &expected_result_A);
        
        date_to_string(datestring, &testdate, MDY);
        sprintf(message, "Testing %s ... ", datestring);
        truefalse = isleapyear(&testdate);
        leap_stats.ttl_tests++;
        
        if(truefalse)
            strcat(message, "Result: Leap year.");
        else
            strcat(message, "Result: Not a leap year.");
        if ((expected_result_A == 1) &&
            (truefalse == expected_result_A)) {
            leap_stats.successful_tests++;
            message_right_justify(message, "PASS", SCREENWIDTH);
        } else if ((expected_result_A == 0) &&
                   (truefalse == expected_result_A)) {
            leap_stats.successful_tests++;
            message_right_justify(message, "PASS", SCREENWIDTH);
        } else {
            message_right_justify(message, "FAIL", SCREENWIDTH);
        }
        
        display_results(message, TESTING);
    }
    display_stats(&leap_stats);
    display_results(NULL, END_FRAME);
    return;
}

void display_stats(struct teststats *printstats)
{
    char message[MAXMESSAGELEN];

    display_results("Test Results", REPORT_RESULTS);
    sprintf(message, "The test battery ran %d tests", printstats->ttl_tests);
    display_results(message, TESTING);
    sprintf(message, "There were %d correct results.", 
            printstats->successful_tests);
    display_results(message, TESTING);
    sprintf(message, "That equates to a %.2f%% success rate.",
            (((float) printstats->successful_tests /
             ((float) printstats->ttl_tests)) * 100));
    display_results(message, TESTING);
    return;
}

void display_results(const char *message, int testphase) 
{
   int rightborder = 0;
   
   switch(testphase){
       case BUILD_FRAME:
           display_frame(message, TOP);
           display_frame(NULL, BOTTOM);
           break;
        case TESTING:
           printf("%c %s", FRAME_SYMB, message);
           rightborder = SCREENWIDTH - (FRAMESIDEWIDTH + 2) - (int) strlen(message);
           print_repeat_char(rightborder, ' ');
           printf("* \n");
           break;
        case REPORT_RESULTS:
           display_frame(message, SECTIONBREAK);
           printf("\n");
           break;
        case BLANK_ROW:
           printf("%c", FRAME_SYMB);
           rightborder = SCREENWIDTH - (FRAMESIDEWIDTH + 1);
           print_repeat_char(rightborder, ' ');
           printf("* \n");
           break;
        case EMPTY_ROW:
           printf("\n");
        case END_FRAME:
           display_frame(NULL, BOTTOM);
            break;
   }
    return;
}

void display_frame(const char *section_name, int framepos)
{
    int SpaceLen = 0;

    switch(framepos){
        case TOP:
            /* Frame Top */
            print_repeat_char(SCREENWIDTH, FRAME_SYMB);
            printf("\n%c", FRAME_SYMB); 

            /*  Frame Title - Centered */
            SpaceLen = (SCREENWIDTH - (FRAMESIDEWIDTH*2) - (int) strlen(section_name))/2;
            if ((SpaceLen % 2) != 0)
                print_repeat_char(SpaceLen, ' ');
            else
                print_repeat_char(SpaceLen + 1, ' ');
            printf("%s", section_name);
            print_repeat_char(SpaceLen, ' ');
            printf("%c\n", FRAME_SYMB); 
            break;
        case SECTIONBREAK:
            print_repeat_char(SCREENWIDTH, FRAME_SYMB);
            printf("\n%c", FRAME_SYMB); 
            /*  Frame Title - Centered */
            SpaceLen = (SCREENWIDTH - (FRAMESIDEWIDTH*2) - (int) strlen(section_name))/2;
            print_repeat_char(SpaceLen, ' ');
            printf("%s", section_name);
            print_repeat_char(SpaceLen, ' ');
            printf("%c", FRAME_SYMB); 
            break;
        case BOTTOM:
            print_repeat_char(SCREENWIDTH, FRAME_SYMB);
            printf("\n"); 
            break;
        default: /* do nothing */
            break;
    }
    return;
}

void print_repeat_char(int count, const char s)
{
    while (count > 0) {
        printf("%c", s);
        count--;
    }
    return;
}

/* add padding to right justify message text */
void message_right_justify(char *text, const char * addedtext, int linewidth) 
{
    unsigned int i;
    int paddingwidth = (linewidth - (int)(strlen(text) + (FRAMESIDEWIDTH +1) * 2 +
                        strlen(addedtext)));
    if (paddingwidth < 1) {
        for (i = (linewidth - (int) strlen(text)); i > 3; i--)
            strcat (text, " ");
        strcat (text, "*\n* ");
        paddingwidth = linewidth - (int) (strlen(addedtext) + 2 + (FRAMESIDEWIDTH * 2));
        for (i = paddingwidth; i > 0 ; i--)
            strcat(text, " ");
        strcat(text, addedtext);
        strcat(text, " ");
    } else {
        for (i = paddingwidth; i > 0 ; i--)
            strcat(text, " ");
        strcat(text, addedtext);
    }
    return;
}
#ifdef UNDEF /* presently this entire source file is removed from compilation
                for testing. */
#define UNDEF


/*
        Add functions to test date validity.

To check if a date is on the calendar:

  if [y,m,d] = d(g(y,m,d))


*/

/* date test */

/* variable declarations */
    struct DateTime begin_date; /* date to begin date calculations */
    struct DateTime end_date; /* date to use for end calculations */
    struct DateTime result_date; /* used to hold new dates resulting from
                                    calculations */
    int day_count; /* number of days before or after a particular deadline */
    char dateinput[11]; /*used to get data from the keyboard */
    char line[80]; /* keyboard buffer */

    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("This is a program to test our various date algorithms.\n");
    printf("please enter the beginning date in the format mm/dd/yyyy");
    printf("\n(Press x to end):\n");

    fgets(line, sizeof(line), stdin);
    sscanf(line, "%d/%d/%d", &begin_date.month, &begin_date.day,
           &begin_date.year);
    printf("/nPlease enter the deadline:");
    fgets(line, sizeof(line), stdin);
    sscanf(line, "%d", &day_count);
    date_offset (&begin_date, &end_date,
                  day_count);
    printf("The deadline is: %d/%d/%d.\n", end_date.month, end_date.day,
           end_date.year);
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    day_count = isholiday (&begin_date);

/* END DATE TEST */

void testsuite (void)
{
    struct DateTime date1; /* variable to test date calculations */
    struct DateTime date2; /* second variable to test date calculations */
    struct DateTime date3; /* third test variable to use for date  */
    int numdays = 35; /* this is a variable to test the date offset function.
                         35 is because the difference between 8/18/11 and
                         9/22/11 = 25 days. */

    date1.year = 2011; /* first date is 8/18/2011 */
    date1.month = 8;
    date1.day = 18;   /* A Thursday */

    date2.year = 2011; /* second date is 9/22/2011 */
    date2.month = 9;   /* this second date should be 25 days after date1 */
    date2.day = 22;    /* A Thursday */


    /* test Sakamoto's formula */

    printf("\n\n=============================================\n");
    printf("The first date is %d/%d/%d\n", date1.month, date1.day, date1.year);
    date1.day_of_week = derive_weekday(&date1);
    printf("The day of the week is ");
    printwkday(date1.day_of_week);
    printf(".\n");
    if (isweekend(&date1))
        printf("That day falls on a weekend.\n");
        else printf ("That day falls on a weekday.\n\n");

    printf("The second date is %d/%d/%d\n", date2.month, date2.day, date2.year);
    date2.day_of_week = derive_weekday(&date2);
    printf("The day of the week is ");
    printwkday(date2.day_of_week);
    printf(".\n");
    if (isweekend(&date2))
        printf("That day falls on a weekend.\n\n");
        else printf ("That day falls on a weekday.\n\n");

    /* test date calculations */

            /* Julian date calcs */

    printf("The Julian date for date1 = %d\n\n", jdncnvrt (&date1));
        printf("The Julian date for date2 = %d\n\n", jdncnvrt (&date2));

    /* difference between two dates */

    printf("There are %d days between date1 and date2\n",
           date_difference(&date1, &date2));
    printf("There are %d days between date2 and date1\n",
           date_difference(&date2, &date1));
    printf("=============================================\n\n");
    /* What date is it in x days? */

    date_offset(&date1, &date3, numdays);
    date3.day_of_week = derive_weekday(&date3;
    printf("What is the due date if the filing is due %d days from %d/%d/%d?\n"
           , numdays, date1.month, date1.day, date1.year);
    printf("The due date is");
    printwkday(date3.day_of_week);
    printf(", %d/%d/%d\n", date3.month, date3.day, date3.year);
    printf("The Julian date for date3 = %d\n\n", jdncnvrt (&date3));
    printf("\n\n=============================================\n");

    return;
}

#endif /* UNDEF */

/* #####   FUNCTION DEFINITIONS  -  LOCAL TO THIS SOURCE FILE   ############# */

