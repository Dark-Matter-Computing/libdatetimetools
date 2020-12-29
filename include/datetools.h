/*
 * Filename: datetools.h
 * Library: libdatetimetools
 *
 * Description: libdatetimetools is a library of useful functions to enable
 * users to perform date and time calculations. One of its primary uses is as a
 * component of solutions to computing chain-based scheduling, as in litigation
 * and project management. 
 *
 * Version: See version.h
 * Created: 08/18/2011 14:24:55
 * Last Modified: Mon Dec 28 21:53:23 2020
 *
 * Author: Thomas H. Vidal (THV), thomashvidal@gmail.com
 * Organization: Dark Matter Computing
 *  
 * Copyright: (c) 2011-2020 - Thomas H. Vidal
 * SPDX-License-Identifier: LGPL-3.0-only
 *
 * Usage: 
 * File Format: Presently the library usese CSV files to import various rules
 * such as holidays.
 * 
 * Restrictions: Presently, the library handles Unix encoded ASCII files fine,
 * but chokes on macOS and Windows files. Handling CR and CRLFs will be
 * addressed in the future. The formula for deriving the day of the week is
 * Sakamoto's.  The formula is accurate for any date in the range September 14,
 * 1752 to December 31, 9999.
 * 
 * Error Handling: Under development
 * 
 * Notes: this private header handles all the data type and file management
 * activities, which should be unnecessary to users of the library.
 */

#ifndef _DATETOOLS_H_INCLUDED_
#define _DATETOOLS_H_INCLUDED_

#include <stdio.h>

/*-----------------------------------------------------------------------------
 * Data Types
 *----------------------------------------------------------------------------*/

/* SYMBOLIC CONSTANTS */

#define WEEKDAYS 7 
#define MINNUMTTLWKS 4 /* All months have at least 4 weeks. */
#define LEAP 1
/* #define TTLMONTHS 13 
#define ALLMONTHS 0 */

#define LASTWEEK 9 /* This is the magic number for a holiday rule that applies
                    to the last x-day of a certain month */

/* Days of the week */
enum DAYS {
    SUNDAY = 0,
    MONDAY = 1,
    TUESDAY = 2,
    WEDNESDAY = 3,
    THURSDAY = 4,
    FRIDAY = 5,
    SATURDAY = 6,
        /* The following "days" are used in various rule computations.
         * Neither is used for actual calendar dates
         */ 
    ALLDAYS = 8, 
    NODAY = 9,
    DAYNOTSET = 999
};

enum MONTHS {
    ALLMONTHS = 0, /* This is the magic number for a holiday rule that applies
                    * to every month of the year, e.g., weekends. Using zero so
                    * that the month array can use 1 to 12 for the month
                    * numbers.
                    */
    JANUARY = 1,
    FEBRUARY = 2,
    MARCH = 3,
    APRIL = 4,
    MAY = 5,
    JUNE = 6,
    JULY = 7,
    AUGUST = 8,
    SEPTEMBER = 9,
    OCTOBER = 10,
    NOVEMBER = 11,
    DECEMBER = 12,
    TTLMONTHS = 13 /* Total months is defined as 13 to account for the code for
                    "holidays" that apply to all months, e.g., Sundays. */
};


/* EXPORTED DATA TYPES */

/*
 * array of months. The array starts with zero, which will never be used, so
 * that subscripts match the actual calendar months 1=january, etc.
 */

    /* TODO Convert to unsigned char to minimize memory usage? */
    /* TODO Consdider using an enum and dropping the extra 13th element. */

static int montharray[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

/* array of total days of each. Starting with zero so subscripts will
 * match up with months.  This is a 2D array to account for leap years.
 */

    /* TODO Convert to unsigned char to minimize memory usage? */

static int daysinmonths[2][13] = {{0, 31, 28, 31, 30, 31,
                                      30, 31, 31, 30, 31, 30, 31},
                                 {0, 31, 29, 31, 30, 31, 30,
                                     31, 31, 30, 31, 30, 31}};

/* A struct to contain the date and time aspects of an event. */

        /*  TODO It might be preferable to break this into a struct for date,
         *  a struct for time, and
         *  then combine them.
         */

struct DateTime
{
    int year;
    int month;
    int day;
    int jdn; /* the Julian Day number for the relevant date */
    enum DAYS day_of_week;

};

struct HolidayNode;

/*-----------------------------------------------------------------------------
 * Activate Rule Handler 
 *----------------------------------------------------------------------------*/

int holiday_rules_open(const char * receivedrulefilename, int close_on_success);

/*-----------------------------------------------------------------------------
 * DATE COMPUTATIONS
 *----------------------------------------------------------------------------*/

/*
 * Name: derive_weekday
 * 
 * Usage/Limitations: This function calculates the day of the week based on
 *   Sakamoto's formula.  The formula is accurate for any date in the range
 *   September 14, 1752 - December 31, 9999
 *
 */

int derive_weekday(struct DateTime *dt);

/* Same as derive_weekday, but this one sets the day_of_week member of the 
 * DateTime struct to the day of the week.
 */
void set_weekday(struct DateTime *dt);

/*
 * Name: isweekend
 *
 * Description: calculates whether a certain date falls on a Saturday or
 * Sunday.  This founction was designed early on in development.  Because
 * most courts treat weekends as holidays, this function is not currently
 * used.  The weeked rules are simply part of the Holiday rules processing
 * tools
 *
 * Parameters: Takes a pointer to a DateTime structure.
 *
 * Return: Returns an integer equal to zero if the date is NOT a weekend, or
 * a 1 if the date IS a weekend.
 *
 */
int isweekend(struct DateTime *dt);

/*
 * Name: isleapyear
 *
 * Description: calculates whether a year is a leap year or not
 *
 * Parameters: Takes a pointer to a DateTime structure.
 *
 * Return: Returns an integer equal to zero if the year is NOT a leap year,
 *  or a 1 if the year IS a leap year.
 *
 */
int isleapyear(struct DateTime *dt);

/*
 * Name: jdncvrt
 *
 * Description: converts a Gregorian calendar date to a Julian Day Number
 * (JDN).  The purpose of using the JDN is to create a unique number for
 * a day that we can use to make date calculations, such as the number of
 * days between two dates or the number of days before or after a
 * specified date. This function does the opposite of the jdn2greg
 * function.
 *
 * Parameters: Takes a pointer to a DateTime structure.
 *
 * Return: Returns an integer with the Julian Date.  Usually this number is
 * not stored for "permanent use" but is used as an argument for
 * another function, such as the date_offset function.
 *
 */
int jdncnvrt(struct DateTime *dt);

/*
 * Name: jdn2greg
 *
 * Description: Converts a Julian Day Number to the Gregorian calendar date.
 *   The purpose of using the JDN is to create a unique number for a day
 *   that we can use to make date calculations, such as the number of days
 *   between two dates or the number of days before or after a specified
 *   date. This function does the opposite of the jdncvrt function.
 *
 * Parameters:  Takes an integer representing a JDN and a pointer to a
 *   DateTime structure.
 *
 */
void jdn2greg(int jdn, struct DateTime *calc_date);

/*
 * Name: date_difference
 *
 * Description: calculates the number of calendar days between two dates.
 *
 * Parameters: The starting date and ending date, both in the form of
 *   pointers to a DateTime struct.
 *
 * Return: Returns an integer which is the result of calculating the number
 *   of calendar days between the two dates.
 *
 */
int date_difference(struct DateTime *date1, struct DateTime *date2);

/*
 * Description: calculates the date after adding or subtracting a specified
 *   number days.
 *
 * Usage: The algorithm EXCLUDES the first date, but counts the end date.
 *
 * Parameters: The starting date, the number of calender days to count, and a
 *   pointer to another DateTime struct to store the result.
 *
 * Return: No return, but the function changes the value of the variable
 *   calc_date (the resulting date) through use of the pointer. The return
 *   value is positive if date1 is before date 2, and negative otherwise.
 */
void date_offset(struct DateTime *orig_date, struct DateTime *calc_date,
                  int numdays);

/*
 * Name: courtday_offset
 *
 * Description: calculates the number of courtdays between two dates. Court
 *   days exclude weekends and holidays.  So the offset does not count those
 *   days.
 *
 * Parameters: The starting date, the number of court days to count, and a
 *   pointer to another DateTime struct to store the result.
 *
 * Return: No return, but the function changes the value of the variable
 *   calc_date (the resulting date) through use of the pointer.
 *
 */
void courtday_offset(struct DateTime *orig_date, struct DateTime *calc_date,
                  int numdays);

/*
 * Name: courtday_difference
 *
 * Description: Counts court days between two dates.
 *
 * Parameters: The starting date and ending date, both in the form of
 *   pointers to a DateTime struct.
 *
 * Return: Returns an integer which is the result of calculating the number
 *   of court days between the two dates. Recall cour days exclude weekends
 *   and holidays. The return value is positive if date1 is before date 2,
 *   and negative otherwise.
 *
 */
int courtday_difference(struct DateTime *date1, struct DateTime *date2);

/*
 * Name: islastxdom
 *
 * Description: calculates whether a particular date is in the last x day
 *               of the month.  (E.g., the last Tuesday of February.)
 *
 * Parameters: Takes a DateTime struct.
 *
 * Returns: An integer 0 = not in last week; 1 = is in last week
 *
 */
int islastxdom(struct DateTime *dt);

/*
 * Name: islastweek
 *
 * Description: calculates whether a particular date is in the last week
 *               of the month.
 *
 * Parameters: Takes a DateTime struct.
 *
 * Returns: An integer 0 = not in last week; 1 = is in last week
 *
 */
int islastweek(struct DateTime *dt);

int isholiday(struct DateTime *dt) ;/* search holiday rules function */

/*-----------------------------------------------------------------------------
 * Output Functions
 *----------------------------------------------------------------------------*/

void printholidayrules(void);

/*
 * Name: wkday_to_string
 * 
 * Description: Gets the weekday corresponding to a day of the enum DAYS. 
 *
 * Parameters: Takes an integer corresponding to a day of the week.
 *
 * Return: A string corresponding to the day of the week, or NULL.
 *
 */

const char* wkday_to_string(int day);

/*
 * Name: month_to_string
 * 
 * Description: Gets the month corresponding to a month of the enum MONTHS. 
 *
 * Parameters: Takes an integer corresponding to a MONTH of the year.
 *
 * Return: A string corresponding to the month of the year, or a NULL pointer.
 *
 */
const char* month_to_string(int month);
#endif	/* _DATETOOLS_H_INCLUDED_ */
