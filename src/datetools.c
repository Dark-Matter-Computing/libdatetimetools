/* SPDX-License-Identifier:  LGPL-3.0-only
 * datetools.c - [short description]
 * [long description]
 *
 * This file is part of libdatetimetools library.
 *
 * Author: Thomas H. Vidal (THV), thomashvidal@gmail.com
 * Organization: Dark Matter Computing
 *
 * Version: 0.0
 * Created: 08/18/2011 14:24:15
 * Last Modified: Tue Dec 15 23:22:02 2020
 *
 * Copyright: (c) 2011-2020 - Thomas H. Vidal, Los Angeles, CA
 * All Rights Reserved.
 *
 *        License: This file is part of libdatetimetools library.
 *
 *                 libdatetimetools is free software: you can redistribute it
 *                 and/or modify it under the terms of the GNU Lesser General
 *                 Public License as published by the Free Software Foundation,
 *                 version 3 of the License.
 *
 *                 libdatetimetools is distributed in the hope that it will be
 *                 useful,but WITHOUT ANY WARRANTY; without even the implied
 *                 warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *                 PURPOSE.  See the GNU Lesser General Public License for
 *                 more details.
 *
 *                 You should have received a copy of the GNU General Public
 *                 License along with libdatetimetools.  If not, see
 *                 <https://www.gnu.org/licenses/>.
 *
 * 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/datetools.h"
#include "/Volumes/development/products/DocketMaster/src/ruleprocessor.h"


/*-----------------------------------------------------------------------------
 *  DAY OF WEEK FUNCTIONS
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

#ifdef UNDEF /* presently this entire the remainng code in this source file is
                removed from compilation for testing purposes. */

/* Add ideas for functions here! */


#endif /* UNDEF */
