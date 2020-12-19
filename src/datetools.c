/*
 * Filename: datetools.c
 * Library: libdatetimetools
 *
 * Description: 
 *
 * Version: 0.0
 * Created: 08/18/2011 14:24:15
 * Last Modified: Fri Dec 18 23:11:43 2020
 *
 * Author: Thomas H. Vidal (THV), thomashvidal@gmail.com
 * Organization: Dark Matter Computing
 *
 * Copyright: (c) 2011-2020 - Thomas H. Vidal, Los Angeles, CA
 * SPDX-License-Identifier:  LGPL-3.0-only
 *
 * Usage: 
 * File Format: 
 * Restrictions: 
 * Error Handling: 
 * References: 
 * Notes: 
 */

#include "../include/datetools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*-----------------------------------------------------------------------------
 * Process Holiday Rules
 *----------------------------------------------------------------------------*/

void initializelist(struct HolidayNode *holidayhashtable[])
{
    int monthctr; /* counter to loop through months */

    for(monthctr = 0; monthctr < MONTHS; monthctr++)
    {
        holidayhashtable[monthctr] = NULL;
    }
    return;
}

struct HolidayNode* addholidayrule(struct HolidayNode *list,
                                    struct HolidayRule *holiday)
{
    struct HolidayNode *new_hrule; /* pointer to new holiday rule */

    new_hrule = malloc(sizeof(struct HolidayNode)); /* creates a new node */

    /* copy the data into the new node */
    new_hrule->rule.month = holiday->month;
    new_hrule->rule.ruletype = holiday->ruletype;
    new_hrule->rule.wkday  = holiday->wkday;
    new_hrule->rule.wknum = holiday->wknum;
    new_hrule->rule.day = holiday->day;
    strcpy(new_hrule->rule.holidayname, holiday->holidayname);
    strcpy(new_hrule->rule.authority, holiday->authority);
    new_hrule->nextrule = list; /* makes the new node point to the current
                                    first node */
    list = new_hrule; /* makes the new node the first node */

    return list;
}

/*
 * File Format: Comma Separated Values.  Fields: Type, Rule, Holiday, Source.
 * The rule field is organized as Month-DayOfWeek-Number.  The "number"
 * sub-field of the rule can contain (1) the week number e.g., 4 for the 4th
 * week, (2) the word "all" for every week- day of the month (e.g., all
 * Saturdays), (3) "last" for the last week-day (e.g., last Monday), or (4)
 * "first" for the first week-day (e.g., first Tuesday).
 */

int processhrule (struct DateTime *dt, struct HolidayNode *rulenode)
{

    switch (rulenode->rule.ruletype)
        {
            case 'a': /* fall through */
            case 'A':
                if (rulenode->rule.day == dt->day)
                    return 1;
                break;
            case 'r': /* fall through */
            case 'R':
                if (rulenode->rule.wkday == dt->day_of_week)
                {
                    /* previous line tests to see if day of week matches. */
                    if ((rulenode->rule.wknum == LASTWEEK) && islastxdom(dt))
                    {
                        return 1;
                    }
                    else if (dt->day >= ((rulenode->rule.wknum-1)*WEEKDAYS+1) &&
                            dt->day <= (rulenode->rule.wknum*WEEKDAYS))
                    {
                        /* Prev. line tests to see if the day is in the
                        proper week. The formula "(wknum-1)*7+1" gets the
                        first day of the applicable week; "wknum*7"
                        calculates the last day of the applicable week.  */

                        return 1;
                    }
                    else
                        return 0;
                }
                break;
            case 'w': /* fall through */
            case 'W':
                if(rulenode->rule.wkday == dt->day_of_week)
                    return 1;
                break;
            default:
                /* fall through */
                break;
        }

    return 0;
}

void closerules(struct HolidayNode *holidayhashtable[])
{
    struct HolidayNode *tempnode;
    int monthctr; /* counter to loop through months */

    for(monthctr = 0; monthctr < MONTHS; monthctr++)
    {
        while (holidayhashtable[monthctr] != NULL)
            /* checks to see if list is empty */
        {
            tempnode = holidayhashtable[monthctr];
            /* sets a temporary pointer to the first node so we don't
                lose it. */
            holidayhashtable[monthctr] = holidayhashtable[monthctr]->nextrule;
            /* makes the next node the new first node */
            free(tempnode); /* frees the memory allocated to the former first
                                node */
        }
    }
    return;
}

/*-----------------------------------------------------------------------------
 * DAY OF WEEK FUNCTIONS
 *----------------------------------------------------------------------------*/

/*
 * Usage/Limitations: This function calculates the day of the week based on
 * Sakamoto's formula.  The formula is accurate for any date in the range
 *                    September 14, 1752 December 31, 9999
 *
 * Explanation: Sakamoto's code in C is as follows:
 *   int dow(int y, int m, int d)
 *      {
 *       static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
 *       y -= m < 3;
 *       return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
 *      }
 *
 * In the above, y, m and d are, respectively the year (e.g., 1988), month
 * (1-12) and day of the month (1-31).  The function returns 0 = Sunday,
 * 1 = Monday, etc.  As the algorithm performs no range checks on the
 * function arguments, unreasonable dates will produce erroneous results
 * or run-time errors. TODO: Add error checking.
 *
 * References. Devised by Tomohiko Sakamoto[1] in 1993.
 * Source. Text copied from Wikipedia.              
 *
 */

int wkday_sakamoto (struct DateTime *dt)
{
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4}; /* I'm not sure what
                                                               this does. */
    int year; /* temporary year variable */
    year = dt->year;
    year -= dt->month < 3;

    return (year + year/4 - year/100 + year/400 +
            t[dt->month-1] + dt->day) % 7;
}

/*-----------------------------------------------------------------------------
 *  DATE CALCULATIONS
 *----------------------------------------------------------------------------*/

int isweekend (struct DateTime *dt)
{
    if (dt->day_of_week == Saturday || Sunday)
        return 1;
        else return 0;
}

int isleapyear(struct DateTime *dt)
{
    return (dt->year%4 == 0 && (dt->year%100 != 0 || dt->year%400 == 0));
}

/*
 * Name: jdncvrt
 *
 * Description: Converts a Gregorian calendar date to a Julian Day Number (JDN).
 *
 * Notes:   This algorithm was published onlne by Aesir Research.
 * The algorithm referenced several interim variables, but it was not clear
 * to me what the variables referenced.  Thus, I have retained the variable
 * names used by Aesir without change.  These  variables are not used in any
 * place other than the function. So I felt it acceptable to use the names
 * "as is." The algorithm is saved in the research subfolder titled "Julian Day
 * Number - Algorithm Forms.txt"
 *
 * The algorithm results in a Julian Day Number, when tested with Jan. 1,
 * 2000, of 2,451,544.  Wikipedia lists the JDN for that date as 2,454,545,
 * and the Julian Date Calculator at www.convertalot.com lists it as
 * 2,451,544.5. (I think the wikipedia entry may just be wrong.)
 *
 * I believe these differences are the result of the fact that pure Julian 
 * Day Numbers go from noon to noon, and account for the time of day.  The 
 * 0.5 difference does not appear to create any cumulative error that
 * spills over to other dates, so this does not present an issue for this
 * function or other functions that use the JDN.
 *
 * References: Algorithm was found at:
 * at http://mysite.verizon.net/aesir_research/date/jdalg2.htm. Aesir
 * research of Onset Massachusetts conducts research into AI, consumer
 * electronics, scientific instrumentations, optics, and mathematical
 * algorithms.
 *
 */

        /* FIXME (Thomas#1#): IMPORTANT - STREAMLINE THE AMOUNT OF FUNCTION
        CALLS TO JDNCNVRT.  PERHAPS INITIALIZE THIS WHEN A DATE IS CREATED TO
        SOME MAGIC NUMBER.  THAT WAY IF THE JDN HAS NOT BE CALCULATED THE
        FUNCTION WILL CALCULATE IT, OTHERWISE IT WILL **NOT** RE-CALCULATE IT.
        THIS SHOULD SAVE SOME CLOCK CYCLES!! */

        /* TODO (Thomas#4#): instead of having jdncnvrt return an int, consider
        having the function simply assign the JDN to the struct member data
        object jdn. */

int jdncnvrt (struct DateTime *dt)
{
    int m; /* m is a temporary variable for month. so we don't change month
                it's only used to calculate f. */
    int z; /* z is the adjusted year, the year starts at March to put leap days
            at the end of the year. */
    float f;

    if (dt->month >= 3)
        z = dt->year;
        else z = dt->year; /* FIXME looks like the if and else do the same thing. */
    m = dt->month;
    if (m < 3){
        m += 12;
        z = z -1;
    }

    f = floor(30.6*m-91.4); /* I'm not sure why, but this variable f is never
                             actually used.  I think it is becaue of the
                             different options that can be selected for the
                             algorithm. */

    return dt->day + (153 * m - 457) / 5 + 365 * z + (z / 4) -
        (z / 100) + (z / 400) + 1721118.5;


    /* dt->day + (153 * m - 457) / 5 + 365 * z + floor(((float)z/4)) -
        floor(((float)z/100)) + floor(((float)z/400)) + 1721118.5; */

}

/*
 * Name: jdn2greg
 *
 * Description: Converts a Julian Day Number to the Gregorian calendar date.
 *
 * Notes:   The algorithm referenced several interim variables, but it was
 * not clear to me what the variables referenced.  Thus, I have retained
 * the variable names used by Dr. Math without change. These variables are
 * not used in any place other than the function. So I felt it acceptable
 * to use the names "as is." The algorithms are stored in the research
 * subfolder titled "Dr Math re Julian to Calendar Date Conversion.txt"
 *
 * For some inexplicable reason, the formula when used exactly as
 * described by Dr. Math results in a calculation that is one day short of
 * the actual JDN resulting from the jdncvrt function.  Consequently, I
 * added a "+1" constant to the formula.
 *
 * I believe the difference may be a result of the use of the greatest
 * integer (the floor function in C) in his algorithm, but my use of
 * simple integer math.  I am not sure.  Needless to say, the algorithm
 * appears to produce accurate results using my correction factor.
 *
 * References: This algorithm was published onlne by "Ask Dr. Math" at
 * http://mathforum.org/library/drmath/view/51907.html. Dr. Math
 * referenced two algorithms: Jean Meeus's algorithm in "Astronomical
 * Formulae for Calculators"; or the Fliegel-Van Flandern algorithm given
 * in the Astronomical Almanac.  I used the Meeus version, which should
 * work for any positive Julian date.
 *
 */

void jdn2greg (int jdn, struct DateTime *calc_date)
{
    int a, b, c, d;
    int alpha;
    int z;
    float e, f, m;

    /* run the intermediate calculations */
    z = jdn+1; /* supposed to be +.5. I used 1 to account for rounding errors */
    f = (jdn+1)-z; /* supposed to be +.5. same as above */
    if (z < 2299161)
        a=z;
        else {
            alpha = ((z-1867216.25)/36524.25);
            a = z + 1 + alpha - (alpha/4);
        }
      b = a + 1524;
      c = (b-122.1)/365.25;
      d = 365.25*c;
      e = floor((b-d)/30.6001);

      /* Use the intermediate calculations to derive the day, month, year */

      calc_date->day = b - d - (30.6001*e) + f + 1;
            /* for the date calculation formula, the final "+1" is not normally
             * included. I use it b/c my result is always one day shy of
             * the correct date.  Perhaps it is because of the use of integer
             * math and because we drop the resulting "0.5" from the jdncnvrt
             * algorithm (again b/c of the integers)? Needless to say, it works.
             */
      
      if (e < 13.5)
        calc_date->month = e-1;
      else calc_date->month = e-13;
      m=calc_date->month;
      if (m >= 3)
        calc_date->year = c-4716;
        else calc_date->year = c - 4715; /* the formula used in the original
                                            algorithm had a constant of 4715 */

   return;
}

int date_difference (struct DateTime *date1, struct DateTime *date2)
{
    return jdncnvrt(date2) - jdncnvrt(date1);
}

void date_offset (struct DateTime *orig_date, struct DateTime *calc_date,
                  int numdays)
{

    jdn2greg ((jdncnvrt(orig_date) + numdays), calc_date);
    return;
}

/*
 * Description: calculates the number of courtdays between two dates. Court
 *   days exclude weekends and holidays.  So the offset does not count those 
 *   days.
 *
 * Algorithm: Function uses two nested while loops.  The outer one counts
 * up or down by numdays.  The inner while-loop cycles through each day
 * starting with the day after the original date.  It tests whether the day
 * falls on a holiday.  If the day falls on a holliday, the inner loop  is
 * incremented, but the outerloop is not. Thus, this function could be
 * modified to report back not only the resulting court day, but the number
 * of actual calendar days between the two dates.
 *
 * NOTE: This function counts forward and backward, so references to
 * incrementing can refer to decrementing; references to next day can refer
 * to prior day, as the case may be; day after can refer to day before.
 */


void courtday_offset (struct DateTime *orig_date, struct DateTime *calc_date,
                  int numdays)
{
    int tempday;
        /* since numdays can only be used to count court-days, tempday
         * is used to check ALL days between original date and end
         * date. numdays will only be incremented when tempday is NOT a holiday
         */

    int test; /* test vaariable to see if a date falls on a holiday or not */
    int fwd_back; /* variable to increment up or down depending on whether
                    we are moving forward or backward on the calendar */

    /* set tempday to the same JDN as orig_date */
    orig_date->jdn = jdncnvrt(orig_date);
    tempday = orig_date->jdn;

    /* If numdays == 0, then there is no need to count anything. */
    if(numdays == 0) {
        calc_date->month = orig_date->month;
        calc_date->day = orig_date->day;
        calc_date->year = orig_date->year;
        calc_date->jdn = orig_date->jdn;

        return;
    }
    /* set fwd_back to 1 or -1 depending on whether we are counting forward or
        backward. */
    if(numdays > 0)
            fwd_back = 1; /* count forward */
    else
        fwd_back = -1; /* count backward */


    /* loop through and count the days */
    while (numdays !=0) { /* while there are days to count */
        test = 1;
        while(test == 1) {
            tempday += fwd_back;
            /* tempday starts out == to the original date; we increment it
             * to point to the very next day. If tempday turns out to be a
             * holiday (see below), tempday is incremented at the beginning
             * of each loop iteration until it points to the next non-holiday
             * date. 
             */

            jdn2greg (tempday, calc_date); /* determine the new day */

            test = isholiday(calc_date); /* is the new day a holiday? */
        }
        numdays -= fwd_back; /* decrease numdays -- this means the function has
                                counted one non-holiday*/
    }
    return;
}

/*
 * Description: Counts court days between two dates.
 *
 * Algorithm: Function uses two nested while loops.  The outer one counts
 * up or down by numdays.  The inner while-loop cycles through each day
 * starting with the day after the end date.  It tests whether the day falls
 * on a holiday.  If the day falls on a holliday, the inner loop  is
 * incremented, but the outerloop is not. Thus, this function could be
 * modified to report back not only the resulting court day, but the number
 * of actual calendar days between the two dates.
 *
 * NOTE: This function counts forward and backward, so references to
 * incrementing can refer to decrementing; references to next day can refer
 * to prior day, as the case may be; day after can refer to day before.
 */

int courtday_difference (struct DateTime *date1, struct DateTime *date2)
{
    struct DateTime testdate; /* structure to hold temporary dates for
                                intermediate testing */

    int test; /* test vaariable to see if a date falls on a holiday or not */

    int fwd_back; /* variable to increment up or down depending on whether
                    we are moving forward or backward on the calendar */

    int count = 0; /* the variable to store the date difference count */

    /* FIXME (Thomas#1#): This function does not work properly if the start
    date falls on a holiday.  MUST ADD ERROR HANDLING. */


    /* set tempday to the same JDN as orig_date */
    date1->jdn = jdncnvrt(date1);
    date2->jdn = jdncnvrt(date2);

    /* If numdays == 0, then there is no need to count anything. */
    if(date1->jdn == date2->jdn) {
        return 0;
    }

    testdate.jdn = date2->jdn; /* set testdate to date2, this is our starting
                                counting position ---> zero! */

    /* set fwd_back to 1 or -1 depending on whether we are counting forward or
        backward. */
    if(date1->jdn > testdate.jdn)
            fwd_back = 1; /* count forward */
    else
        fwd_back = -1; /* count backward */


    /* loop through and count the days */
    while (testdate.jdn != date1->jdn) { /* while there are days to count */
        test = 1;
        while(test == 1) {
            testdate.jdn += fwd_back;
                /* we increment testdate to point to the very next (or
                 * previous) day.  if testdate turns out to be a holiday (see
                 * below), testdate is incremented again at the beginning of
                 * the next loop iteration. The loop iterates until testdate
                 * points to a non-holiday date.
                 */

            jdn2greg (testdate.jdn, &testdate); /* determine the new day */

            test = isholiday(&testdate);
                /* is the new day a holiday? If so, don't count it and test the
                 * next (or previous) day
                 */
        }
        count -= fwd_back; /* testdate was not a holiday, so count it! */
    }
    return count;
}

/*
 * Description: calculates whether a particular date is in the last x day
 *               of the month.  (E.g., the last Tuesday of February.)        
 *
 * Algorithm: Step one is to simply test the day# of the argument date.
 *   If the number is less than WEEKDAYS * (MINNUMTTLWKS-1) (i.e., 22),then
 *   it cannot possibly be in the last week and function returns.  If not,
 *   Step 2 begins. The job is simply to (1) determine the last week of the
 *   date-argument's month, (2) identify the last Xday of the month, and (3)
 *   compare the last Xday with the argument's Xday. If they are the same,
 *   the argument date is the last Xday of the month.
 *
 */

int islastxdom (struct DateTime *dt)
{
    struct DateTime tempdate; /* date struct used to store interim values */
    int daycount;
        /* counter variable to count how many days difference between the day
         * of the week of the last week of the first month to the laset x-day
         * of the current month.
         */

    if (dt->day < (WEEKDAYS*(MINNUMTTLWKS-1))) {
        /* if the date of month is less then 22 it cannot be the last week.*/
        return 0;
    }
    else {
        /* Set tempdate to first day of next month */
        tempdate.day = 1;
        if (dt->month == 12) {
            tempdate.month = 1;
            tempdate.year = dt->year + 1;
        }

        else {
            tempdate.month = dt->month + 1;
            tempdate.year = dt->year;
        }
        dt->day_of_week = wkday_sakamoto (dt);
        tempdate.day_of_week = wkday_sakamoto (&tempdate);
            /* calculate day of week of tempdate */
        jdncnvrt (&tempdate); /*calculate Julian Day Number of tempdate */

        if (tempdate.day_of_week > dt->day_of_week)
            daycount = (tempdate.day_of_week - dt->day_of_week) * -1;
        else
            daycount = (tempdate.day_of_week - dt->day_of_week + WEEKDAYS) * -1;

        date_offset (&tempdate, &tempdate, daycount);

        if (tempdate.day == dt->day)
            return 1;
        else
            return 0;
    }

    return 0;
}


/*
 * Description: calculates whether a particular date is in the last week
 * of the month.
 *
 * Note: not currently used / not currently coded
 *
 * Algorithm: Step one is to simply test the day# of the argument date.
 * If the number is less than WEEKDAYS * (MINNUMTTLWKS-1) (i.e., 22),then
 * it cannot possibly be in the last week and function returns.  If not,
 * Step 2 begins. Calling function knows that the day of the week of the
 * date argument (let's call that Xday) matches the day of the week of a
 * specific holiday. This function's job is simply to (1) determine the
 * last week of date-argument's month, (2) identify the last Xday of the
 * month, and (3) compare the last Xday with the argument's Xday. If they
 * are the same, the argument date is the last Xday of the month.
 *
 *   ASSUMES FIRST DAY OF WEEK IS SUNDAY!
 */

int islastweek (struct DateTime *dt)
{
    struct DateTime tempdate; 
    int daycount; 

    if (dt->day < (WEEKDAYS*(MINNUMTTLWKS-1))) { /* if the date of month is less
            then 22 it cannot be the last week. */
        return 0;
    }
    else if (dt->day == daysinmonths[isleapyear(dt)][dt->month]) {
        /* Determine whether the day is the last day of the month. If the date
         * is the last day of the month it is by definition in the last week.
         * The array subscripts work like this.  The first dimension is a one
         * or a zero depending on whether this is a leap year. The second
         * dimension is the month of the date argument.
         */
    
        return 1;
    }
    else {
        /* set tempdate to the last date of the month */
        tempdate.year = dt->year;
        tempdate.month = dt->month;
        tempdate.day = daysinmonths[isleapyear(dt)][dt->month];

        /* calculate day of week of dt and tempdate */
        dt->day_of_week = wkday_sakamoto (dt);
        tempdate.day_of_week = wkday_sakamoto (&tempdate);

        /* calculate number of days between the argument and the last day of
            the month. */
        daycount = date_difference(dt, &tempdate);
        if (daycount > 7)
            return 0;
        else if (daycount <= (int) dt->day_of_week - (int) tempdate.day_of_week)
            /* FIXME I need to check this because I'm comparing signed and
             * unsigned types w/o the cast  */
            return 1;
        else
            return 0;
    }

    return 0;
}

int isholiday (struct DateTime *dt)
{
    struct HolidayNode *tempnode;

    /* First, calculate whether an ALLMONTHS rule applies and whether this date
    falls on a weekend */

    dt->day_of_week = wkday_sakamoto (dt);
    tempnode = holidayhashtable[ALLMONTHS-1];
    while(tempnode != NULL)
        {
            if (processhrule(dt,tempnode) == 1)
                return 1;
            tempnode = tempnode->nextrule;
        }

    /* Second, calculate whether there are any holidays on the month of the
        argument's date */

    tempnode = holidayhashtable[dt->month-1];
    while(tempnode != NULL)
        {
            if (processhrule(dt,tempnode) == 1)
                return 1;
            tempnode = tempnode->nextrule;
        }

    return 0;
}

void printholidayrules(struct HolidayNode *holidayhashtable[])
{
    struct HolidayNode *tempnode;
    int monthctr; /* counter to loop through months */

    for(monthctr = 0; monthctr < MONTHS; monthctr++)
    {
        tempnode = holidayhashtable[monthctr];
        /* sets a temporary pointer to the first node so we traverse
            the list. */

        printf("-------------------------------------------------------\n");
        if (tempnode != NULL)
        {
            switch (monthctr)
            {
            case 0: /* January rules */
                printf("This holiday rules for January are as follows:\n");
                break;
            case 1:
                printf("This holiday rules for February are as follows:\n");
                break;
            case 2:
                printf("This holiday rules for March are as follows:\n");
                break;
            case 3:
                printf("This holiday rules for April are as follows:\n");
                break;
            case 4:
                printf("This holiday rules for May are as follows:\n");
                break;
            case 5:
                printf("This holiday rules for June are as follows:\n");
                break;
            case 6:
                printf("This holiday rules for July are as follows:\n");
                break;
            case 7:
                printf("This holiday rules for August are as follows:\n");
                break;
            case 8:
                printf("This holiday rules for September are as follows:\n");
                break;
            case 9:
                printf("This holiday rules for October are as follows:\n");
                break;
            case 10:
                printf("This holiday rules for November are as follows:\n");
                break;
            case 11:
                printf("This holiday rules for December are as follows:\n");
                break;
            case 12:
                printf("This holiday rules that apply to all months");
                printf(" are as follows:\n");
                break;
            default:
                printf("\n\n\n ERROR BAD MONTH.\n");
                break;
            }
        }
        else
        {
            switch (monthctr)
            {
            case 0: /* January rules */
                printf("No holidays in January.\n");
                break;
            case 1:
                printf("No holidays in February.\n");
                break;
            case 2:
                printf("No holidays in March.\n");
                break;
            case 3:
                printf("No holidays in April.\n");
                break;
            case 4:
                printf("No holidays in May.\n");
                break;
            case 5:
                printf("No holidays in June.\n");
                break;
            case 6:
                printf("No holidays in July.\n");
                break;
            case 7:
                printf("No holidays in August.\n");
                break;
            case 8:
                printf("No holidays in September.\n");
                break;
            case 9:
                printf("No holidays in October.\n");
                break;
            case 10:
                printf("No holidays in November.\n");
                break;
            case 11:
                printf("No holidays in December.\n");
                break;
            case 12:
                printf("No holidays in apply to all months.");
                break;
            default:
                printf("\n\n\n ERROR BAD MONTH.\n");
                break;
            }
        }
        while (tempnode != NULL) /* checks to see if list is empty */
        {
            printf("The applicable holiday is %s.\n",
                   tempnode->rule.holidayname);
            printf("The applicable ruletype is %c.\n",
                   tempnode->rule.ruletype);
            if ((tempnode->rule.ruletype != 'a') &&
                    (tempnode->rule.ruletype != 'A'))
            {
                printf("The applicable weekday is %d.\n",
                       tempnode->rule.wkday);
                printf("The applicable weeknumber is %d.\n",
                       tempnode->rule.wknum);
            }
            else
            {
                printf("The Day is %d.\n", tempnode->rule.day);
            }
            printf("The governing authority is %s.\n",
                   tempnode->rule.authority);
            printf("\n");
            /* move to the next node */
            tempnode = tempnode->nextrule;
        }

    }
    return;
}

/*
 * Name: printwkday
 *
 * Description: Prints the weekday corresponding to a day of the enum days.
 *
 * Parameters: Takes an integer corresponding to a day of the week.
 *
 * Return: Nothing.  It just prints the day.
 * 
 */

void printwkday (int day)
{
    switch (day) {
        case Sunday:
            printf(" Sunday");
            break;
        case Monday:
            printf(" Monday");
            break;
        case Tuesday:
            printf(" Tuesday");
            break;
        case Wednesday:
            printf(" Wednesday");
            break;
        case Thursday:
            printf(" Thursday");
            break;
        case Friday:
            printf(" Friday");
            break;
        case Saturday:
            printf(" Saturday");
            break;
        default:
            /* do nothing */
            break;
    }

    return;
}

#ifdef UNDEF /* presently this entire the remainng code in this source file is
                removed from compilation for testing purposes. */

/* Add ideas for functions here! */


#endif /* UNDEF */
