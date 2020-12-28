/*
 * Filename: testsuite.c
 * Library: libdatetimetools
 *
 * Description:  Testsuite.c creates a number of functions for testing the
 * libdatetimetools library's functionality and quality.
 *
 * Version: 1.0.0.0
 * Created:  Mon Dec 21 21:17:02 2020
 * Last Modified: Sun Dec 27 21:26:07 2020
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
#include "../include/datetools.h"
#include "../include/timetools.h"
#include "testsuite.h"


/* Functions */

void testsuite_check_calcs(const char *receivedtestfile)
{
    FILE *testfile;
    char line[100];
    struct teststats calc_stats;


    calc_stats.ttl_tests = 0;
    calc_stats.successful_tests = 0;

    /* open file */
    testfile = fopen(receivedtestfile, "r");

    /* read and discard the headers */
    fgets(line, sizeof(line), testfile);

    test_loop(CALCS, testfile, &calc_stats);  
    print_stats(&calc_stats);

    return;
}


void testsuite_check_deriveday(const char *receivedtestfile)
{
    FILE *testfile;
    char line[100];
    struct teststats deriveday_stats;


    deriveday_stats.ttl_tests = 0;
    deriveday_stats.successful_tests = 0;

    /* open file */
    testfile = fopen(receivedtestfile, "r");
    /* read and discard the headers */
    fgets(line, sizeof(line), testfile);
 /*     
            case EOF:
                break;
            case 0: / fall through /
            case 1: / fall through /
            case 2: / fall through /
            case 3:
                printf("There are formatting problems with the data in"
                        "file: %s.\n", receivedtestfile);
                */
    test_loop(DERIVEDATES, testfile, &deriveday_stats);
    print_stats(&deriveday_stats);
    
    
    return;
}

void testsuite_checkholidays(void)
{
    struct DateTime begin_date; /* date to begin date calculations */
    struct DateTime end_date; /* date to use for end calculations */
    struct DateTime result_date; /* used to hold new dates resulting from
                                    calculations */
    int day_count; /* number of days before or after a particular deadline */
    char dateinput[11]; /*used to get data from the keyboard */
    char line[80]; /* keyboard buffer */

    printf("\n\n\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("This function tests our various date algorithms.\n");
    printf("The first date tests a \"weekend\" rule.\n");
    printf("The date is October 8, 2011 (Saturday).\n");
    begin_date.month = 10;
    begin_date.day = 8;
    begin_date.year = 2011;
    holidayprinttest(&begin_date);

    printf("The next date tests a \"weekend\" rule.\n");
    printf("The date is October 9, 2011 (Sunday).\n");
    begin_date.month = 10;
    begin_date.day = 9;
    begin_date.year = 2011;
    holidayprinttest(&begin_date);

    printf("The next date tests an \"absolute\" rule.\n");
    printf("The date is January 1, 2010 (Friday, New Year's Day).\n");
    begin_date.month = 1;
    begin_date.day = 1;
    begin_date.year = 2010;
    holidayprinttest(&begin_date);

    printf("The next date tests an \"absolute\" rule.\n");
    printf("The date is December 25, 2014 (Thursday, Christmas Day).\n");
    begin_date.month = 12;
    begin_date.day = 25;
    begin_date.year = 2014;
    holidayprinttest(&begin_date);

    printf("The next date tests a \"relative\" rule.\n");
    printf("The date is November 24, 2011 (Thursday, Thanksgiving Day).\n");
    begin_date.month = 11;
    begin_date.day = 24;
    begin_date.year = 2011;
    holidayprinttest(&begin_date);

    printf("The next date tests a \"relative\" rule.\n");
    printf("The date is May 28, 2011 (Memorial Day).\n");
    begin_date.month = 5;
    begin_date.day = 28;
    begin_date.year = 2012;
    holidayprinttest(&begin_date);


    printf("\n\n\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    /* commented out ...
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
    end commented out section. */
    return;
}

void holidayprinttest(struct DateTime *dt)
{
    printf("%d/%d/%d ", dt->month, dt->day, dt->year);
    if (isholiday(dt) != 0)
        printf("is a holiday.\n\n");
    else
        printf("is NOT a holiday.\n\n");

    return;

}

void test_loop(enum TESTFILETYPES testtype, FILE *testfile,
               struct teststats *stats)
{
    struct DateTime testdate; 
    char line[100];
    int count;
    int result = 0;
    int truefalse = 0;
    int expected_result_A = 0;
    int expected_result_B = 0;

    while (fgets(line, sizeof(line), testfile) != NULL) {
        switch (testtype)
        {
            case DERIVEDATES:
                count = sscanf(line, "%d,%d,%d,%d", &testdate.year,
                              &testdate.month, &testdate.day,
                              &expected_result_A);
                break;
            case CALCS:
                count = sscanf(line, "%d,%d,%d,%d,%d", &testdate.year,
                               &testdate.month, &testdate.day,
                               &expected_result_A, &expected_result_B);
                testdate.day_of_week = derive_weekday(&testdate);
                break;

        }
        switch (count) {
            case 4:
                result = derive_weekday(&testdate);
                printf("Testing the following (D/M/Y): %d/%d/%d.\n",
                        testdate.month, testdate.day, testdate.year);
                if (result >= 0){
                    printf("The test was in range. Weekday is %s.\t", wkday_to_string(result));
                } else {
                    printf("The test date was out of range. "
                           "No computation performed.\t");
                }

                if ((expected_result_A == -1) &&
                    (result == expected_result_A)) {
                    stats->successful_tests++;
                    printf("PASS\n");
                } else if ((expected_result_A == 0) && (result >= 0)) {
                    stats->successful_tests++;
                    printf("PASS\n");
                } else {
                    printf ("FAIL FAIL\n");
                }
                stats->ttl_tests++;
                break;
            case 5:
                printf("\n\n#Calc Test# dd/mm/yy = %d/%d/%d.\n", testdate.month,
                        testdate.day, testdate.year);
                printf("Is that the last %s of %s?\n",
                        wkday_to_string(testdate.day_of_week),
                        month_to_string(testdate.month));

                truefalse = islastxdom(&testdate);
                truefalse = islastxdom(&testdate);
                if(truefalse)
                    printf("Yes. It is the last %s of the %s.\t",
                           wkday_to_string(testdate.day_of_week),
                                           month_to_string(testdate.month));
                else
                    printf("No. It is NOT last %s of %s.\t",
                           wkday_to_string(testdate.day_of_week),
                           month_to_string(testdate.month));

                if ((expected_result_A == 1) &&
                    (truefalse == expected_result_A)) {
                    stats->successful_tests++;
                    printf("PASS\n");
                } else if ((expected_result_A == 0) &&
                           (truefalse == expected_result_A)) {
                    stats->successful_tests++;
                    printf("PASS\n");
                } else {
                    printf("FAIL FAIL\n");
                }
                stats->ttl_tests++;

                printf("Is that date in the last week of the month?\n");

                truefalse = islastweek(&testdate);
                
                if(truefalse)
                    printf("Yes. It is in the last week of %s.\t",
                            month_to_string(testdate.month));
                else
                    printf("No. It is NOT in the last week of %s.\t",
                            month_to_string(testdate.month));

                if ((expected_result_B == 1) &&
                    (truefalse == expected_result_B)) {
                    stats->successful_tests++;
                    printf("PASS\n");
                } else if ((expected_result_B == 0) &&
                          (truefalse == expected_result_B)) {
                    stats->successful_tests++;
                    printf("PASS\n");
                } else {
                    printf("FAIL FAIL\n");
                }
                stats->ttl_tests++;
                break;
            default:
                break;
        }
    } 
    return;
}

void print_stats(struct teststats *printstats)
{
    printf ("\n\n######## Test Results ########\n");
    /* add switch to list the test type that was run. */
    printf ("## You ran %d tests.\n", printstats->ttl_tests);
    printf ("## There were %d correct results.\n",
            printstats->successful_tests);
    printf ("## That equates to a %.2f%%.\n\n",
            (((float) printstats->successful_tests /
             ((float) printstats->ttl_tests)) * 100));
    return;
}

#ifdef UNDEF /* presently this entire source file is removed from compilation
                for testing. */
#define UNDEF

void testsuite_check_leap(const char *receivedtestfile)
{
    struct DateTime testtate;

    /* test the isleapyear function */
    printf("\n\n##############################################");
    printf("\nLeap Year Function Test.\n");

    testdate.day = 1;
    testdate.month = 1;
    testdate.year = 1600;
    printf("\n\n#TEST# mm/dd/yy = %d/%d/%d.\n", testdate.month, testdate.day,
           testdate.year);
    truefalse = isleapyear(&testdate);
    if(truefalse)
        printf("Yes, %d is a leap year.\n", testdate.year);
    else
        printf("No, %d is a leap year.\n", testdate.year);


    testdate.day = 1;
    testdate.month = 1;
    testdate.year = 2000;
    printf("\n\n#TEST# mm/dd/yy = %d/%d/%d.\n", testdate.month, testdate.day,
           testdate.year);
    truefalse = isleapyear(&testdate);
    if(truefalse)
        printf("Yes, %d is a leap year.\n", testdate.year);
    else
        printf("No, %d is not a leap year.\n", testdate.year);

    testdate.jdn = jdncnvrt(&testdate);
    printf("\n\n#test# 1/1/2000 has a JDN of %d", testdate.jdn);

    testdate.day = 1;
    testdate.month = 1;
    testdate.year = 1700;
    printf("\n\n#TEST# mm/dd/yy = %d/%d/%d.\n", testdate.month, testdate.day,
           testdate.year);
    truefalse = isleapyear(&testdate);
    if(truefalse)
        printf("Yes, %d is a leap year.\n", testdate.year);
    else
        printf("No, %d is not a leap year.\n", testdate.year);


    testdate.day = 1;
    testdate.month = 1;
    testdate.year = 1800;
    printf("\n\n#TEST# mm/dd/yy = %d/%d/%d.\n", testdate.month, testdate.day,
           testdate.year);
    truefalse = isleapyear(&testdate);
    if(truefalse)
        printf("Yes, %d is a leap year.\n", testdate.year);
    else
        printf("No, %d is not a leap year.\n", testdate.year);


    testdate.day = 1;
    testdate.month = 1;
    testdate.year = 2100;
    printf("\n\n#TEST# mm/dd/yy = %d/%d/%d.\n", testdate.month, testdate.day,
           testdate.year);
    truefalse = isleapyear(&testdate);
    if(truefalse)
        printf("Yes, %d is a leap year.\n", testdate.year);
    else
        printf("No, %d is not a leap year.\n", testdate.year);


    testdate.day = 1;
    testdate.month = 1;
    testdate.year = 2108;
    printf("\n\n#TEST# mm/dd/yy = %d/%d/%d.\n", testdate.month, testdate.day,
           testdate.year);
    truefalse = isleapyear(&testdate);
    if(truefalse)
        printf("Yes, %d is a leap year.\n", testdate.year);
    else
        printf("No, %d is not a leap year.\n", testdate.year);

    return;
}
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

